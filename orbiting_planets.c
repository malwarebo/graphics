#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float angle = 0.0f;
const float PI = 3.14159265f;

typedef struct {
    float radius;      // Radius of the planet
    float orbitRadius; // Distance from the sun
    float speed;       // Rotation speed
    float tilt;        // Orbit tilt angle
    float position;    // Current position in orbit (in radians)
    float red, green, blue; // Planet color
} Planet;

// Define solar system with sun and planets
Planet sun = {0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.8f, 0.0f}; // Yellow sun
Planet planets[] = {
    {0.05f, 0.4f, 4.0f, 0.0f, 0.0f, 0.8f, 0.2f, 0.2f},   // Mercury (red)
    {0.08f, 0.6f, 3.0f, 0.0f, 0.0f, 0.9f, 0.9f, 0.2f},   // Venus (yellow)
    {0.1f,  0.8f, 2.0f, 0.1f, 0.0f, 0.2f, 0.4f, 0.9f},   // Earth (blue)
    {0.07f, 1.0f, 1.5f, 0.2f, 0.0f, 0.9f, 0.3f, 0.2f},   // Mars (red)
    {0.15f, 1.3f, 1.0f, 0.1f, 0.0f, 0.9f, 0.7f, 0.3f}    // Jupiter (orange)
};

int numPlanets = sizeof(planets) / sizeof(Planet);

void drawSphere(float radius, int slices, int stacks) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluSphere(quadric, radius, slices, stacks);
    gluDeleteQuadric(quadric);
}

void drawOrbit(float radius, float tilt) {
    const int segments = 100;
    
    glPushMatrix();
    glRotatef(tilt * 180.0f / PI, 1.0f, 0.0f, 0.0f); // Apply tilt
    glBegin(GL_LINE_LOOP);
    glColor3f(0.5f, 0.5f, 0.5f); // Gray color for orbits
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
    
    // Set planet color
    glColor3f(planet.red, planet.green, planet.blue);
    
    // Draw the planet
    drawSphere(planet.radius, 20, 20);
    
    glPopMatrix();
}

void drawSolarSystem() {
    // Draw the sun
    glPushMatrix();
    glColor3f(sun.red, sun.green, sun.blue);
    drawSphere(sun.radius, 30, 30);
    glPopMatrix();
    
    // Draw orbits and planets
    for (int i = 0; i < numPlanets; i++) {
        drawOrbit(planets[i].orbitRadius, planets[i].tilt);
        drawPlanet(planets[i]);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Position the camera to view the solar system
    gluLookAt(0.0f, 1.5f, 2.0f,  // Eye position
              0.0f, 0.0f, 0.0f,  // Look at position
              0.0f, 1.0f, 0.0f); // Up vector
    
    // Apply global rotation for viewing different angles
    glRotatef(angle * 0.1f, 0.0f, 1.0f, 0.0f);
    
    drawSolarSystem();
    
    glutSwapBuffers();
}

void update(int value) {
    angle += 1.0f;
    if (angle > 360) {
        angle -= 360;
    }
    
    // Update planet positions
    for (int i = 0; i < numPlanets; i++) {
        planets[i].position += planets[i].speed * 0.01f;
        if (planets[i].position > 2.0f * PI) {
            planets[i].position -= 2.0f * PI;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q') {
        printf("Quit requested. Exiting...\n");
        exit(0);
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
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // Dark blue background (space)
    glEnable(GL_DEPTH_TEST);
    
    // Enable lighting for better 3D appearance
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    // Set up light properties
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Light from the sun position
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // White light
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // Soft ambient light
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLUT Version: %d\n", GLUT_API_VERSION);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    int window = glutCreateWindow("Solar System Simulation");
    printf("Window created with ID: %d\n", window);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, update, 0);
    glutKeyboardFunc(keyboard);
    
    init();
    
    printf("Starting main loop...\n");
    glutMainLoop();
    return 0;
}
