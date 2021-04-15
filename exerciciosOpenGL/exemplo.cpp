/*
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  csg.c
 *  This program demonstrates the use of the OpenGL depth function for
 *   generating images equivalent to the ones obtained from CSG.
 *
 *  Ting (20/03/2010)
 */
#include <GL/glut.h>
#include <stdlib.h>

GLfloat dx, y, z;
/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init(void) 
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 0.0, 2.0, 1.0, 0.0 };

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

   dx=y=z=0.0;
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
   glRotatef(dx, 1.0, 0.0, 0.0);
   glRotatef(y , 0.0, 1.0, 0.0);
   glRotatef(z, 0.0, 0.0, 1.0);
   glPushMatrix();
   glRotatef(-90., 1.0,0.0,0.0);
   glutSolidCone (0.75, 2.0, 20, 16);
   glPopMatrix();
   glPushMatrix();
   glTranslatef(0.0, 1.5, 0.0);
   glutSolidSphere (0.5, 10, 10);
   glPopMatrix();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-2.5, 2.5, -2.5*(GLfloat)h/(GLfloat)w,
         2.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-2.5*(GLfloat)w/(GLfloat)h,
         2.5*(GLfloat)w/(GLfloat)h, -2.5, 2.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
 }

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'x':
         dx += 10.;
		 if (dx > 360.) dx = 0.;
         glutPostRedisplay();
         break;
      case 'y':
         y += 10.;
		 if (y > 360.) y = 0.;
         glutPostRedisplay();
         break;
      case 'z':
         z += 10.;
		 if (z > 360.) z = 0.;
         glutPostRedisplay();
         break;
      case 'r':
         x = y = z = 0.;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}