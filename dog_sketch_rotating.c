#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float angle = 0.0f;

void drawSphere(float radius, int slices, int stacks) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluSphere(quadric, radius, slices, stacks);
    gluDeleteQuadric(quadric);
}

void drawCylinder(float radius, float height, int slices, int stacks) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluCylinder(quadric, radius, radius, height, slices, stacks);
    gluDeleteQuadric(quadric);
}

void drawDog() {
    // Body
    glPushMatrix();
    glColor3f(0.8f, 0.6f, 0.4f);  // Light brown
    glScalef(1.0f, 0.6f, 0.8f);
    drawSphere(0.5f, 20, 20);
    glPopMatrix();

    // Head
    glPushMatrix();
    glTranslatef(0.4f, 0.3f, 0.0f);
    glColor3f(0.7f, 0.5f, 0.3f);  // Darker brown
    drawSphere(0.25f, 20, 20);
    glPopMatrix();

    // Snout
    glPushMatrix();
    glTranslatef(0.6f, 0.25f, 0.0f);
    glColor3f(0.6f, 0.4f, 0.2f);  // Even darker brown
    glScalef(0.2f, 0.15f, 0.15f);
    drawSphere(0.5f, 20, 20);
    glPopMatrix();

    // Eyes
    glPushMatrix();
    glTranslatef(0.5f, 0.35f, 0.1f);
    glColor3f(0.1f, 0.1f, 0.1f);  // Dark grey
    drawSphere(0.05f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, 0.35f, -0.1f);
    glColor3f(0.1f, 0.1f, 0.1f);  // Dark grey
    drawSphere(0.05f, 10, 10);
    glPopMatrix();

    // Ears
    glPushMatrix();
    glTranslatef(0.3f, 0.5f, 0.15f);
    glRotatef(-30, 0.0f, 0.0f, 1.0f);
    glColor3f(0.6f, 0.4f, 0.2f);  // Dark brown
    glScalef(0.1f, 0.2f, 0.05f);
    drawSphere(0.5f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3f, 0.5f, -0.15f);
    glRotatef(-30, 0.0f, 0.0f, 1.0f);
    glColor3f(0.6f, 0.4f, 0.2f);  // Dark brown
    glScalef(0.1f, 0.2f, 0.05f);
    drawSphere(0.5f, 20, 20);
    glPopMatrix();

    // Legs
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        float x = (i < 2) ? 0.3f : -0.3f;
        float z = (i % 2 == 0) ? 0.2f : -0.2f;
        glTranslatef(x, -0.3f, z);
        glColor3f(0.7f, 0.5f, 0.3f);  // Brown
        drawCylinder(0.06f, 0.3f, 10, 10);
        glPopMatrix();
    }

    // Tail
    glPushMatrix();
    glTranslatef(-0.5f, 0.1f, 0.0f);
    glRotatef(45, 0.0f, 0.0f, 1.0f);
    glColor3f(0.8f, 0.6f, 0.4f);  // Light brown
    drawCylinder(0.05f, 0.3f, 10, 10);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(0.0f, 0.0f, 2.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
    
    glRotatef(angle, 0.0f, 1.0f, 0.0f);  // Rotate around Y-axis
    
    drawDog();
    
    glutSwapBuffers();
}

void update(int value) {
    angle += 2.0f;
    if (angle > 360) {
        angle -= 360;
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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLUT Version: %d\n", GLUT_API_VERSION);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    int window = glutCreateWindow("Rotating 3D Dog Sketch");
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