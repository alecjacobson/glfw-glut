#ifndef GLFW_GLUT_H
#define GLFW_GLUT_H

#define GLFW_INCLUDE_GLU
#ifndef _WIN32
  #define GLFW_INCLUDE_GLCOREARB
#endif

#include <GLFW/glfw3.h>

#include <chrono>
#include <string>
#include <cstddef>
#include <iostream>
#include <thread>
#include <cassert>
#include <functional>
#ifdef _WIN32
#  include <ctime>
#else
#  include <sys/time.h>
#endif

/* Display mode bit masks. */
#define GLUT_RGB			0
#define GLUT_RGBA			GLUT_RGB
//#define GLUT_INDEX			1
//#define GLUT_SINGLE			0
#define GLUT_DOUBLE			2
//#define GLUT_ACCUM			4
//#define GLUT_ALPHA			8
#define GLUT_DEPTH			16
//#define GLUT_STENCIL			32
#define GLUT_MULTISAMPLE		128
//#define GLUT_STEREO			256
//#define GLUT_LUMINANCE			512
//#define GLUT_NO_RECOVERY    1024
#define GLUT_3_2_CORE_PROFILE	2048
/* Mouse buttons. */
#define GLUT_LEFT_BUTTON		0
#define GLUT_MIDDLE_BUTTON		1
#define GLUT_RIGHT_BUTTON		2
#define GLUT_WHEEL_UP			3
#define GLUT_WHEEL_DOWN		4
#define GLUT_WHEEL_LEFT	5
#define GLUT_WHEEL_RIGHT 6
/* Mouse button  state. */
#define GLUT_DOWN			0
#define GLUT_UP				1
/* function keys */
#define GLUT_KEY_F1			1
#define GLUT_KEY_F2			2
#define GLUT_KEY_F3			3
#define GLUT_KEY_F4			4
#define GLUT_KEY_F5			5
#define GLUT_KEY_F6			6
#define GLUT_KEY_F7			7
#define GLUT_KEY_F8			8
#define GLUT_KEY_F9			9
#define GLUT_KEY_F10			10
#define GLUT_KEY_F11			11
#define GLUT_KEY_F12			12
/* directional keys */
#define GLUT_KEY_LEFT			100
#define GLUT_KEY_UP			101
#define GLUT_KEY_RIGHT			102
#define GLUT_KEY_DOWN			103
#define GLUT_KEY_PAGE_UP		104
#define GLUT_KEY_PAGE_DOWN		105
#define GLUT_KEY_HOME			106
#define GLUT_KEY_END			107
#define GLUT_KEY_INSERT			108
#define GLUT_LEFT			0
#define GLUT_ENTERED			1
#define GLUT_ACTIVE_SHIFT               1
#define GLUT_ACTIVE_CTRL                2
#define GLUT_ACTIVE_ALT                 4
#define GLUT_ACTIVE_COMMAND             8
/* glutSetCursor parameters. */
/* Basic arrows. */
#define GLUT_CURSOR_RIGHT_ARROW		0
#define GLUT_CURSOR_LEFT_ARROW		1
/* Symbolic cursor shapes. */
#define GLUT_CURSOR_INFO		2
#define GLUT_CURSOR_DESTROY		3
#define GLUT_CURSOR_HELP		4
#define GLUT_CURSOR_CYCLE		5
#define GLUT_CURSOR_SPRAY		6
#define GLUT_CURSOR_WAIT		7
#define GLUT_CURSOR_TEXT		8
#define GLUT_CURSOR_CROSSHAIR		9
/* Directional cursors. */
#define GLUT_CURSOR_UP_DOWN		10
#define GLUT_CURSOR_LEFT_RIGHT		11
/* Sizing cursors. */
#define GLUT_CURSOR_TOP_SIDE		12
#define GLUT_CURSOR_BOTTOM_SIDE		13
#define GLUT_CURSOR_LEFT_SIDE		14
#define GLUT_CURSOR_RIGHT_SIDE		15
#define GLUT_CURSOR_TOP_LEFT_CORNER	16
#define GLUT_CURSOR_TOP_RIGHT_CORNER	17
#define GLUT_CURSOR_BOTTOM_RIGHT_CORNER	18
#define GLUT_CURSOR_BOTTOM_LEFT_CORNER	19
/* Inherit from parent window. */
#define GLUT_CURSOR_INHERIT		100
/* Blank cursor. */
#define GLUT_CURSOR_NONE		101
/* Fullscreen crosshair (if available). */
#define GLUT_CURSOR_FULL_CROSSHAIR	102
extern void *glutStrokeRoman;
extern void *glutStrokeMonoRoman;
#define GLUT_STROKE_ROMAN		(&glutStrokeRoman)
#define GLUT_STROKE_MONO_ROMAN		(&glutStrokeMonoRoman)

void glutInit(int *argcp, char **argv);
void glutInitDisplayMode(unsigned int mode);
void glutInitDisplayString(const char * string);
void glutInitWindowSize(int width, int height);
void glutInitWindowPosition(int width, int height);
int glutCreateWindow(const char *name);
void glutMainLoop();
void glutDisplayFunc(void (*func)(void));
void glutSwapBuffers();
void glutPostRedisplay();
void glutReshapeFunc(void (*func)(int width, int height));
void glutSpecialFunc(void (*func)(int key, int x, int y));
void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
void glutMouseFunc(void (*func)(int button, int state, int x, int y));
void glutMotionFunc(void (*func)(int x, int y));
void glutPassiveMotionFunc(void (*func)(int x, int y));
int glutGetModifiers();
void glutSetCursor(int cursor);
extern "C"
{
void glutStrokeCharacter(void *font, int character);
int glutStrokeWidth(void *font, int c);
}
#endif
