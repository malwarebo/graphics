#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float angle = 0.0f;

void drawHexagonalPrism() {
  float radius = 0.5f;
  float height = 0.25f;
  int num_vertices = 6;

  // Top face
  glBegin(GL_POLYGON);
  glColor3f(1.0f, 0.0f, 0.0f); // Red
  for (int i = 0; i < num_vertices; i++) {
    float theta = 2.0f * M_PI * (float)i / (float)num_vertices;
    float x = radius * cosf(theta);
    float y = radius * sinf(theta);
    glVertex3f(x, y, height);
  }
  glEnd();

  // Bottom face
  glBegin(GL_POLYGON);
  glColor3f(0.0f, 1.0f, 0.0f); // Green
  for (int i = 0; i < num_vertices; i++) {
    float theta = 2.0f * M_PI * (float)i / (float)num_vertices;
    float x = radius * cosf(theta);
    float y = radius * sinf(theta);
    glVertex3f(x, y, -height);
  }
  glEnd();

  // Side faces
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.0f, 0.0f, 1.0f); // Blue
  for (int i = 0; i <= num_vertices; i++) {
    float theta = 2.0f * M_PI * (float)i / (float)num_vertices;
    float x = radius * cosf(theta);
    float y = radius * sinf(theta);
    glVertex3f(x, y, height);
    glVertex3f(x, y, -height);
  }
  glEnd();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  glRotatef(angle, 1.0f, 1.0f, 1.0f); // Rotate around all axes

  drawHexagonalPrism();

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
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("GLUT Version: %d\n", GLUT_API_VERSION);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  int window = glutCreateWindow("Rotating 3D Hexagonal Prism");
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
