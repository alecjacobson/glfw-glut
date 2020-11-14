#include "glfw_glut.h"

void display()
{
  glClearColor(0.56471,0.84706,0.76863,1.0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glutSwapBuffers();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize(512, 512);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("A blank riptide colored glfw-glut window");
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
