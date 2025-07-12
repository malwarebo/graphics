#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float angle = 0.0f;
const float PI = 3.14159265f;

// Planet structure with more realistic properties
typedef struct {
    float radius;           // Radius of the planet
    float orbitRadius;      // Distance from the sun (astronomical units)
    float speed;            // Orbital speed (relative)
    float tilt;             // Orbit tilt angle
    float axialTilt;        // Planet's axis tilt
    float rotationSpeed;    // Planet's rotation speed
    float rotationAngle;    // Current rotation angle
    float position;         // Current position in orbit (in radians)
    float red, green, blue; // Planet color
    int rings;              // 0 for no rings, 1 for rings
    float ringInner;        // Inner radius of rings
    float ringOuter;        // Outer radius of rings
    float ringRed, ringGreen, ringBlue;  // Ring colors
} Planet;

// Define solar system with realistic relative properties
Planet sun = {0.15f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.9f, 0.2f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // Yellow sun

Planet planets[] = {
    // Mercury (small grey)
    {0.038f, 0.4f, 4.74f, 0.0f, 0.03f, 0.5f, 0.0f, 0.0f, 0.7f, 0.7f, 0.7f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    
    // Venus (yellowish)
    {0.095f, 0.7f, 3.5f, 0.05f, 3.1f, 0.2f, 0.0f, 0.0f, 0.9f, 0.8f, 0.5f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    
    // Earth (blue)
    {0.1f, 1.0f, 2.98f, 0.0f, 0.41f, 1.0f, 0.0f, 0.0f, 0.3f, 0.5f, 0.9f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    
    // Mars (red)
    {0.053f, 1.5f, 2.41f, 0.03f, 0.44f, 0.98f, 0.0f, 0.0f, 0.9f, 0.3f, 0.2f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    
    // Jupiter (orange with bands)
    {0.28f, 2.2f, 1.31f, 0.02f, 0.05f, 2.4f, 0.0f, 0.0f, 0.9f, 0.7f, 0.4f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    
    // Saturn (yellow with rings)
    {0.24f, 3.0f, 0.97f, 0.06f, 0.47f, 2.2f, 0.0f, 0.0f, 0.9f, 0.8f, 0.5f, 1, 0.3f, 0.5f, 0.9f, 0.8f, 0.6f},
    
    // Uranus (light blue with rings)
    {0.18f, 3.7f, 0.68f, 0.13f, 1.71f, 1.4f, 0.0f, 0.0f, 0.5f, 0.8f, 0.9f, 1, 0.2f, 0.3f, 0.5f, 0.8f, 0.9f},
    
    // Neptune (dark blue)
    {0.17f, 4.3f, 0.54f, 0.1f, 0.49f, 1.5f, 0.0f, 0.0f, 0.2f, 0.4f, 0.8f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}
};

int numPlanets = sizeof(planets) / sizeof(Planet);

// Moon data
Planet moon = {0.027f, 0.16f, 10.0f, 0.05f, 0.1f, 0.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
int earthIndex = 2; // Earth is at index 2 in the planets array

float cameraDistance = 5.0f;
float cameraHeight = 2.0f;
float cameraAngle = 0.0f;
int viewMode = 0; // 0 = default, 1 = top-down, 2 = follow Earth

void drawSphere(float radius, int slices, int stacks) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluSphere(quadric, radius, slices, stacks);
    gluDeleteQuadric(quadric);
}

void drawRings(float innerRadius, float outerRadius) {
    const int segments = 100;
    const int rings = 10;
    float ringStep = (outerRadius - innerRadius) / rings;
    
    glBegin(GL_QUADS);
    for (int r = 0; r < rings; r++) {
        float inner = innerRadius + r * ringStep;
        float outer = innerRadius + (r + 1) * ringStep;
        
        for (int i = 0; i < segments; i++) {
            float theta1 = 2.0f * PI * (float)i / (float)segments;
            float theta2 = 2.0f * PI * (float)(i + 1) / (float)segments;
            
            float x1 = inner * cosf(theta1);
            float z1 = inner * sinf(theta1);
            float x2 = outer * cosf(theta1);
            float z2 = outer * sinf(theta1);
            float x3 = outer * cosf(theta2);
            float z3 = outer * sinf(theta2);
            float x4 = inner * cosf(theta2);
            float z4 = inner * sinf(theta2);
            
            glVertex3f(x1, 0.0f, z1);
            glVertex3f(x2, 0.0f, z2);
            glVertex3f(x3, 0.0f, z3);
            glVertex3f(x4, 0.0f, z4);
        }
    }
    glEnd();
}

void drawOrbit(float radius, float tilt) {
    const int segments = 100;
    
    glPushMatrix();
    glRotatef(tilt * 180.0f / PI, 1.0f, 0.0f, 0.0f); // Apply tilt
    glBegin(GL_LINE_LOOP);
    glColor3f(0.3f, 0.3f, 0.3f); // Gray color for orbits
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * PI * (float)i / (float)segments;
        float x = radius * cosf(theta);
        float y = 0.0f;
        float z = radius * sinf(theta);
        glVertex3f(x, y, z);
    }
    glEnd();
    glPopMatrix();
}

void drawPlanet(Planet planet) {
    glPushMatrix();
    
    // Apply tilt to the orbit
    glRotatef(planet.tilt * 180.0f / PI, 1.0f, 0.0f, 0.0f);
    
    // Calculate position on the orbit
    float x = planet.orbitRadius * cosf(planet.position);
    float z = planet.orbitRadius * sinf(planet.position);
    glTranslatef(x, 0.0f, z);
    
    // Apply axial tilt and rotation
    glRotatef(planet.axialTilt * 180.0f / PI, 0.0f, 0.0f, 1.0f);
    glRotatef(planet.rotationAngle, 0.0f, 1.0f, 0.0f);
    
    // Set planet color
    glColor3f(planet.red, planet.green, planet.blue);
    
    // Draw the planet
    drawSphere(planet.radius, 30, 30);
    
    // Draw rings if the planet has them
    if (planet.rings) {
        glColor3f(planet.ringRed, planet.ringGreen, planet.ringBlue);
        glPushMatrix();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Orient rings around the planet
        drawRings(planet.radius * planet.ringInner, planet.radius * planet.ringOuter);
        glPopMatrix();
    }
    
    glPopMatrix();
}

void drawMoon() {
    glPushMatrix();
    
    // Calculate Earth's position
    glRotatef(planets[earthIndex].tilt * 180.0f / PI, 1.0f, 0.0f, 0.0f);
    float earthX = planets[earthIndex].orbitRadius * cosf(planets[earthIndex].position);
    float earthZ = planets[earthIndex].orbitRadius * sinf(planets[earthIndex].position);
    glTranslatef(earthX, 0.0f, earthZ);
    
    // Draw moon orbit
    drawOrbit(moon.orbitRadius, moon.tilt);
    
    // Draw the moon
    glRotatef(moon.tilt * 180.0f / PI, 1.0f, 0.0f, 0.0f);
    float moonX = moon.orbitRadius * cosf(moon.position);
    float moonZ = moon.orbitRadius * sinf(moon.position);
    glTranslatef(moonX, 0.0f, moonZ);
    
    glColor3f(moon.red, moon.green, moon.blue);
    drawSphere(moon.radius, 20, 20);
    
    glPopMatrix();
}

void drawSolarSystem() {
    // Draw the sun with glow effect
    glPushMatrix();
    glDisable(GL_LIGHTING); // Disable lighting for the sun to make it glow
    glColor3f(sun.red, sun.green, sun.blue);
    drawSphere(sun.radius, 40, 40);
    
    // Add a subtle glow effect
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.8f, 0.2f, 0.2f);
    drawSphere(sun.radius * 1.2f, 40, 40);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    
    // Draw orbits and planets
    for (int i = 0; i < numPlanets; i++) {
        drawOrbit(planets[i].orbitRadius, planets[i].tilt);
        drawPlanet(planets[i]);
    }
    
    // Draw the moon
    drawMoon();
}

void setupCamera() {
    glLoadIdentity();
    
    switch (viewMode) {
        case 0: // Default orbital view
            gluLookAt(
                cameraDistance * cos(cameraAngle), cameraHeight, cameraDistance * sin(cameraAngle),
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f
            );
            break;
            
        case 1: // Top-down view
            gluLookAt(
                0.0f, 5.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, -1.0f
            );
            break;
            
        case 2: { // Earth following view
            // Calculate Earth's position
            float earthX = planets[earthIndex].orbitRadius * cosf(planets[earthIndex].position);
            float earthZ = planets[earthIndex].orbitRadius * sinf(planets[earthIndex].position);
            
            // Position camera slightly behind and above Earth
            float lookAngle = planets[earthIndex].position - PI/8;
            float camDist = 0.3f;
            float camX = earthX - camDist * cosf(lookAngle);
            float camZ = earthZ - camDist * sinf(lookAngle);
            
            gluLookAt(
                camX, 0.15f, camZ,
                earthX, 0.0f, earthZ,
                0.0f, 1.0f, 0.0f
            );
            break;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    setupCamera();
    drawSolarSystem();
    
    glutSwapBuffers();
}

void update(int value) {
    // Update camera angle
    cameraAngle += 0.002f;
    if (cameraAngle > 2 * PI) {
        cameraAngle -= 2 * PI;
    }
    
    // Update planet positions and rotations
    for (int i = 0; i < numPlanets; i++) {
        planets[i].position += planets[i].speed * 0.005f;
        if (planets[i].position > 2.0f * PI) {
            planets[i].position -= 2.0f * PI;
        }
        
        planets[i].rotationAngle += planets[i].rotationSpeed * 0.5f;
        if (planets[i].rotationAngle > 360.0f) {
            planets[i].rotationAngle -= 360.0f;
        }
    }
    
    // Update moon
    moon.position += moon.speed * 0.005f;
    if (moon.position > 2.0f * PI) {
        moon.position -= 2.0f * PI;
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'q':
        case 'Q':
            printf("Quit requested. Exiting...\n");
            exit(0);
            break;
        case 'v':
        case 'V':
            // Switch view mode
            viewMode = (viewMode + 1) % 3;
            printf("View mode: %d\n", viewMode);
            break;
        case '+':
            // Zoom in
            cameraDistance -= 0.2f;
            if (cameraDistance < 1.0f) cameraDistance = 1.0f;
            break;
        case '-':
            // Zoom out
            cameraDistance += 0.2f;
            if (cameraDistance > 10.0f) cameraDistance = 10.0f;
            break;
        case 'w':
        case 'W':
            // Move camera up
            cameraHeight += 0.2f;
            if (cameraHeight > 4.0f) cameraHeight = 4.0f;
            break;
        case 's':
        case 'S':
            // Move camera down
            cameraHeight -= 0.2f;
            if (cameraHeight < -4.0f) cameraHeight = -4.0f;
            break;
    }
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            cameraAngle -= 0.1f;
            break;
        case GLUT_KEY_RIGHT:
            cameraAngle += 0.1f;
            break;
    }
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f); // Dark blue background (space)
    glEnable(GL_DEPTH_TEST);
    
    // Enable lighting for better 3D appearance
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    // Set up light properties
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Light from the sun position
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };  // Slightly yellow light
    GLfloat light_ambient[] = { 0.1f, 0.1f, 0.15f, 1.0f }; // Very soft ambient light
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White specular highlights
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    // Add stars in the background
    glEnable(GL_POINT_SMOOTH);
    
    printf("Realistic Solar System Simulation Started\n");
    printf("Controls:\n");
    printf("  Q: Quit\n");
    printf("  V: Switch view mode (orbital, top-down, Earth-following)\n");
    printf("  +/-: Zoom in/out\n");
    printf("  W/S: Move camera up/down\n");
    printf("  Left/Right arrows: Orbit around the scene\n");
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Realistic Solar System Simulation");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    init();
    
    glutMainLoop();
    return 0;
}
