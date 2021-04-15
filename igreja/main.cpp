#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

#define M_PI 3.14159265358979323846

// GLOBALS
float YELLOW, RED, BLACK, BLUE, GREEN;

// FUNCTIONS
void init(void);
void reshape(int w, int h);
void display(void);

//
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(800, 600); 
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Igejinha");
  init();
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  RED = 0.0;
  BLUE = 0.0;
  GREEN = 0.0;
}

void reshape(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glViewport(0, 0, w, h);

  glOrtho(-w/2, w/2, -h/2, h/2, -1, 1);

  glMatrixMode(GL_MODELVIEW);
}

void desenhaCirculo(float raio, int num_linhas,bool preenchido)
{
  int i;
  GLfloat angulo;
  
  angulo = 2 * M_PI / num_linhas;

  glColor3f(0.0, 0.9, 0.0);
  if(preenchido)
      glBegin(GL_TRIANGLE_FAN);    
  else glBegin(GL_LINE_LOOP);
  
  for(i = 1; i <= num_linhas; i++) {
    glVertex2f(raio*cos(i * angulo) , raio*sin(i * angulo) );
  }
  glEnd(); 
}


void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  // LEFT TOWER
  glColor3f(0.0, 0.0, 0.9);
  glBegin(GL_QUADS);
    glVertex2f(-100, 0);
    glVertex2f(-200,0);
    glVertex2f(-200, -180);
    glVertex2f(-100, -180);
  glEnd();


  // RIGHT TOWER
  glColor3f(0.0, 0.0, 0.9);
  glBegin(GL_QUADS);
    glVertex2f(200, 0);
    glVertex2f(100, 0);
    glVertex2f(100, -180);
    glVertex2f(200, -180);
  glEnd();

  // LEFT ROOF
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_TRIANGLES);
    glVertex2f(-100, 0);
    glVertex2f(-150, 70);
    glVertex2f(-200, 0);
  glEnd();

  // RIGHT ROOF
  glColor3f(9.0, 0.0, 0.0);
  glBegin(GL_TRIANGLES);
    glVertex2f(200, 0);
    glVertex2f(150, 70);
    glVertex2f(100, 0);
  glEnd();

  // CENTER WALL
  glColor3f(0.9, 0.9, 0.0);
  glBegin(GL_QUADS);
    glVertex2f(100, 0);
    glVertex2f(-100, 0);
    glVertex2f(-100, -180);
    glVertex2f(100, -180);
  glEnd();

  // BLACK DOOR
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex2f(45, -70);
    glVertex2f(-45, -70);
    glVertex2f(-45, -180);
    glVertex2f(45, -180);
  glEnd();

  // CENTER ROOF
  glColor3f(0.9, 0.0, 0.0);
  glBegin(GL_TRIANGLES);
    glVertex2f(100, 0);
    glVertex2f(0, 170);
    glVertex2f(-100, 0);
  glEnd();

  // CENTER CROSS 1
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex2f(0, 170);
    glVertex2f(0, 250);
  glEnd();
  // CENTER CROSS 2
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex2f(-30, 220);
    glVertex2f(30, 220);
  glEnd();

  // LEFT CROSS 1
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex2f(-150, 70);
    glVertex2f(-150, 150);
  glEnd();
  // LEFT CROSS 2
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex2f(-180, 120);
    glVertex2f(-120, 120);
  glEnd();

  // RIGHT CROSS 1
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex2f(150, 70);
    glVertex2f(150, 150);
  glEnd();
  // RIGHT CROSS 2
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex2f(180, 120);
    glVertex2f(120, 120);
  glEnd();

  // WINDOW LEFT
  glTranslated(-150, -50, 0); // Translade for left
  desenhaCirculo(30, 100, 1);

  // WINDOW RIGHT
  glTranslated(300, 0, 0); // Translade for center again + translate for right
  desenhaCirculo(30, 100, 1);

  glFlush();
}