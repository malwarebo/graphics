#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float angle = 0.0f;
float jumpHeight = 0.0f;
const float PI = 3.14159265f;

typedef struct {
  float x, y, z;
} Point3D;

Point3D skeleton[9] = {
    {0, 0, 0},         // Hip
    {0, 0.5f, 0},      // Chest
    {0, 0.8f, 0},      // Head
    {-0.2f, 0.4f, 0},  // Left shoulder
    {-0.4f, 0, 0},     // Left hand
    {0.2f, 0.4f, 0},   // Right shoulder
    {0.4f, 0, 0},      // Right hand
    {-0.1f, -0.5f, 0}, // Left foot
    {0.1f, -0.5f, 0}   // Right foot
};

void drawLine(Point3D start, Point3D end) {
  glBegin(GL_LINES);
  glVertex3f(start.x, start.y, start.z);
  glVertex3f(end.x, end.y, end.z);
  glEnd();
}

void drawSkeleton() {
  glColor3f(1.0f, 1.0f, 1.0f); // White color for the skeleton

  // Draw body
  drawLine(skeleton[0], skeleton[1]); // Hip to chest
  drawLine(skeleton[1], skeleton[2]); // Chest to head

  // Draw arms
  drawLine(skeleton[1], skeleton[3]); // Chest to left shoulder
  drawLine(skeleton[3], skeleton[4]); // Left shoulder to left hand
  drawLine(skeleton[1], skeleton[5]); // Chest to right shoulder
  drawLine(skeleton[5], skeleton[6]); // Right shoulder to right hand

  // Draw legs
  drawLine(skeleton[0], skeleton[7]); // Hip to left foot
  drawLine(skeleton[0], skeleton[8]); // Hip to right foot

  // Draw head (simple sphere)
  glPushMatrix();
  glTranslatef(skeleton[2].x, skeleton[2].y, skeleton[2].z);
  glutWireSphere(0.1, 10, 10);
  glPopMatrix();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis

  // Apply jump height
  glTranslatef(0.0f, jumpHeight, 0.0f);

  drawSkeleton();

  glutSwapBuffers();
}

void update(int value) {
  angle += 2.0f;
  if (angle > 360) {
    angle -= 360;
  }

  // Update jump height
  jumpHeight = 0.1f * sinf(angle * PI / 180.0f);

  // Animate arms and legs
  float armAngle = 30.0f * sinf(angle * PI / 90.0f);
  float legAngle = 15.0f * sinf(angle * PI / 90.0f);

  skeleton[4].y = -0.4f + 0.1f * sinf((angle + 90) * PI / 90.0f); // Left hand
  skeleton[6].y = -0.4f + 0.1f * sinf(angle * PI / 90.0f);        // Right hand
  skeleton[7].y = -0.5f + 0.1f * sinf(angle * PI / 90.0f);        // Left foot
  skeleton[8].y = -0.5f + 0.1f * sinf((angle + 90) * PI / 90.0f); // Right foot

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
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
  glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("3D Jumping Skeleton");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(25, update, 0);
  glutKeyboardFunc(keyboard);

  init();

  glutMainLoop();
  return 0;
}
