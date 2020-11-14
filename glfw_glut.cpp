#include "glfw_glut.h"
#include <cmath>

namespace GLFW_GLUT
{
  int g_width = -1;
  int g_height = -1;
  int g_init_window_x=-1, g_init_window_y=-1;
  int g_num_windows = 0;
  bool g_is_animating = false;
  bool g_wait_flag = true;
  int g_x,g_y;
  int g_mod;
  bool g_is_down = false;
  GLFWwindow * g_window = NULL;
  std::function<void(void)> g_display_callback = [](){};
  std::function<void(int,int)> g_reshape_callback = [](int,int){};
  std::function<void(unsigned char,int,int)> g_keyboard_callback = [](unsigned char,int,int){};
  std::function<void(int,int,int)> g_special_callback= [](int,int,int){};
  std::function<void(int,int,int,int)> g_mouse_callback = [](int,int,int,int){};
  std::function<void(int,int)> g_motion_callback = [](int,int){};
  std::function<void(int,int)> g_passive_motion_callback = [](int,int){};
  //GLFWmonitor * g_monitor = NULL;
}

void glutInit(int *argcp, char **argv)
{
  // Returned success flag is ignored
  glfwInit();
}

void glutInitDisplayMode(unsigned int mode)
{
  if(mode & GLUT_3_2_CORE_PROFILE)
  {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  }
  if(mode & GLUT_RGBA)
  {
    glfwWindowHint(GLFW_RED_BITS,  8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS,8);
    glfwWindowHint(GLFW_ALPHA_BITS,8);
  }
  if(mode & GLUT_DEPTH)
  {
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
  }
  if(mode & GLUT_DOUBLE)
  {
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
  }
  if(mode & GLUT_MULTISAMPLE)
  {
    glfwWindowHint(GLFW_SAMPLES, 8);
  }
}
void glutInitDisplayString(const char * cstring)
{
  std::string str = cstring;
  if(str.find("rgba") || str.find("rgb"))
  {
    glfwWindowHint(GLFW_RED_BITS,  8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS,8);
    glfwWindowHint(GLFW_ALPHA_BITS,8);
  }
  if(str.find("depth"))
  {
    glfwWindowHint(GLFW_DEPTH_BITS,24);
  }
  if(str.find("double"))
  {
    glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
  }
  int sam = str.find("samples>=");
  if(sam >= 0)
  {
    int num_sams = -1;
    assert(str.length() > (sam+9));
    if(sscanf(str.c_str()+sam+9,"%d",&num_sams))
    {
      glfwWindowHint(GLFW_SAMPLES,num_sams);
    }
  }
}

void glutInitWindowSize(int width, int height)
{
  GLFW_GLUT::g_width = width;
  GLFW_GLUT::g_height = height;
}

void glutInitWindowPosition(int x, int y)
{
  GLFW_GLUT::g_init_window_x = x;
  GLFW_GLUT::g_init_window_y = y;
}

int glutCreateWindow(const char *name)
{
  using namespace GLFW_GLUT;
  if(g_num_windows > 0)
  {
    std::cerr<<"GLFW_GLUT Error: no support for multiple windows"<<std::endl;
    return -1;
  }
  int winID = -1;
  g_window = glfwCreateWindow(g_width,g_height,name,nullptr,nullptr);
  if(g_window)
  {
    winID = 0;
    glfwMakeContextCurrent(g_window);
    glfwSetInputMode(g_window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    if(g_init_window_x >= 0 && g_init_window_y >= 0)
    {
      glfwSetWindowPos(g_window,g_init_window_x,g_init_window_y);
    }
    // set up inputs
    glfwSetMouseButtonCallback(
      g_window,
      [](GLFWwindow* window, int button, int action, int mod)
      {
        g_mod = mod;
        switch(button)
        {
          default:
          case GLFW_MOUSE_BUTTON_1:
          {
            button = GLUT_LEFT_BUTTON;
            break;
          }
          case GLFW_MOUSE_BUTTON_2:
          {
            button = GLUT_RIGHT_BUTTON;
            break;
          }
          case GLFW_MOUSE_BUTTON_3:
          {
            button = GLUT_MIDDLE_BUTTON;
            break;
          }
        }
        switch(action)
        {
          case GLFW_PRESS:
          {
            action = GLUT_DOWN;
            g_is_down = true;
            break;
          }
          default:
          {
            action = GLUT_UP;
            g_is_down = false;
            break;
          }
        }
        g_mouse_callback(button,action,g_x,g_y);
      });
    glfwSetCursorPosCallback(
      g_window,
      [](GLFWwindow * window, double x, double y)
      {
        // Remember position on every move
        g_x = x;
        g_y = y;
        if(g_is_down)
        {
          g_motion_callback(g_x,g_y);
        }else
        {
          g_passive_motion_callback(g_x,g_y);
        }
      });
    glfwSetCharModsCallback(
      g_window,
      [](GLFWwindow * window, unsigned int code, int mod)
      {
        //std::cout<<"CharModes: "<<std::endl;
        //std::cout<<"  code: "<<code<<" char: "<<(char)code<<std::endl;
        GLFW_GLUT::g_mod = mod;
        switch(code)
        {
          default:
          {
            g_keyboard_callback(static_cast<unsigned char>(code),g_x,g_y);
            break;
          }
          case GLFW_KEY_F1:
            g_special_callback(GLUT_KEY_F1,g_x,g_y);
            break;
          case GLFW_KEY_F2:
            g_special_callback(GLUT_KEY_F2,g_x,g_y);
            break;
          case GLFW_KEY_F3:
            g_special_callback(GLUT_KEY_F3,g_x,g_y);
            break;
          case GLFW_KEY_F4:
            g_special_callback(GLUT_KEY_F4,g_x,g_y);
            break;
          case GLFW_KEY_F5:
            g_special_callback(GLUT_KEY_F5,g_x,g_y);
            break;
          case GLFW_KEY_F6:
            g_special_callback(GLUT_KEY_F6,g_x,g_y);
            break;
          case GLFW_KEY_F7:
            g_special_callback(GLUT_KEY_F7,g_x,g_y);
            break;
          case GLFW_KEY_F8:
            g_special_callback(GLUT_KEY_F8,g_x,g_y);
            break;
          case GLFW_KEY_F9:
            g_special_callback(GLUT_KEY_F9,g_x,g_y);
            break;
          case GLFW_KEY_F10:
            g_special_callback(GLUT_KEY_F10,g_x,g_y);
            break;
          case GLFW_KEY_F11:
            g_special_callback(GLUT_KEY_F11,g_x,g_y);
            break;
          case GLFW_KEY_F12:
            g_special_callback(GLUT_KEY_F12,g_x,g_y);
            break;
          case GLFW_KEY_LEFT:
            g_special_callback(GLUT_KEY_LEFT,g_x,g_y);
            break;
          case GLFW_KEY_UP:
            g_special_callback(GLUT_KEY_UP,g_x,g_y);
            break;
          case GLFW_KEY_RIGHT:
            g_special_callback(GLUT_KEY_RIGHT,g_x,g_y);
            break;
          case GLFW_KEY_DOWN:
            g_special_callback(GLUT_KEY_DOWN,g_x,g_y);
            break;
          case GLFW_KEY_PAGE_UP:
            g_special_callback(GLUT_KEY_PAGE_UP,g_x,g_y);
            break;
          case GLFW_KEY_PAGE_DOWN:
            g_special_callback(GLUT_KEY_PAGE_DOWN,g_x,g_y);
            break;
          case GLFW_KEY_HOME:
            g_special_callback(GLUT_KEY_HOME,g_x,g_y);
            break;
          case GLFW_KEY_END:
            g_special_callback(GLUT_KEY_END,g_x,g_y);
            break;
          case GLFW_KEY_INSERT:
            g_special_callback(GLUT_KEY_INSERT,g_x,g_y);
            break;
        }
      });
    glfwSetKeyCallback(
      g_window,
      [](GLFWwindow * window, int key, int scancode, int action, int mod)
      {
        // Keep track of mods
        g_mod = mod;
        // And send other key down actions to g_keyboard_callback ...
        if(action == GLFW_PRESS || action == GLFW_REPEAT)
        {
          //std::cout<<"SetKey:"<<std::endl;
          switch(key)
          {
            default:
              //std::cout<<"  Key: "<<key<<", scancode: "<<scancode<<", char: "<<(char)key<<std::endl;
              // ignore
              break;
            case 67:
              if(mod & GLFW_MOD_CONTROL)
              {
                g_keyboard_callback(char(3),g_x,g_y);
              }
              break;
            case GLFW_KEY_BACKSPACE:
              g_keyboard_callback(char(127),g_x,g_y);
              break;
            case GLFW_KEY_ESCAPE:
              g_keyboard_callback(char(27),g_x,g_y);
              break;
          }
        }
      });
    glfwSetScrollCallback(
      g_window,
      [](GLFWwindow * window, double x, double y)
      {
        static double x_surplus = 0;
        static double y_surplus = 0;
        const auto & apply = 
          [](double & x, double & x_surplus, int right, int left)
          {
            x += x_surplus;
            x_surplus = x-round(x);
            x = round(x-x_surplus);
            int dir = x>0 ? right : left;
            x = fabs(x);
            while(x != 0)
            {
              g_mouse_callback(dir,GLUT_DOWN,g_x,g_y);
              x--;
            }
          };
        apply(x,x_surplus,GLUT_WHEEL_RIGHT,GLUT_WHEEL_LEFT);
        apply(y,y_surplus,GLUT_WHEEL_UP,GLUT_WHEEL_DOWN);
      });
    glfwSetWindowSizeCallback(
      g_window,
      [](GLFWwindow * window, int w, int h){ g_reshape_callback(w,h);});
  }
  return winID;
}

void glutMainLoop()
{
  using namespace GLFW_GLUT;
  const auto & get_seconds = []()->double
  {
#ifdef _WIN32
    return double(clock())/CLOCKS_PER_SEC;
#else
    timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec + time.tv_usec / 1e6;
#endif
  };

  // GLUT apps expect that window reshape callback is called at least once
  {
    int width_window, height_window;
    glfwGetWindowSize(g_window, &width_window, &height_window);
    g_reshape_callback(width_window,height_window);
  }

  if(g_is_animating)
  {
    while(!glfwWindowShouldClose(g_window))
    {
      double tic = get_seconds();
      g_display_callback();
      glfwSwapBuffers(g_window);
      glfwPollEvents();
      // In microseconds
      double duration = 1000000.*(get_seconds()-tic);
      // Throttle at 30 fps
      const double min_duration = 1000000./30.;
      if(duration<min_duration)
      {
        std::this_thread::sleep_for(std::chrono::microseconds((int)(min_duration-duration)));
      }
    }
  }else
  {
    while(!glfwWindowShouldClose(g_window))
    {
      // Unless g_display_callback calls glutPostRedisplay() then we'll wait on
      // events
      g_wait_flag = true;
      g_display_callback();
      // It'd be nice to use glfwWaitEvents() but can't figure out how to make
      // glutPostRedisplay() it jump out
      while(!glfwWindowShouldClose(g_window))
      {
        // Make sure events are polled at least once between each draw
        glfwPollEvents();
        if(g_wait_flag)
        {
          std::this_thread::sleep_for(std::chrono::microseconds(33000));
        }else
        {
          break;
        }
      }
    }
  }
}

void glutDisplayFunc(void (*func)(void))
{
  GLFW_GLUT::g_display_callback = [func](){ func();};
}

void glutSwapBuffers()
{
  glfwSwapBuffers(GLFW_GLUT::g_window);
}

void glutPostRedisplay()
{
  GLFW_GLUT::g_wait_flag = false;
}

void glutReshapeFunc(void (*func)(int w, int h))
{
  GLFW_GLUT::g_reshape_callback = [func](int w, int h){func(w,h);};
}

void glutSpecialFunc(void (*func)(int key, int x, int y))
{
  GLFW_GLUT::g_special_callback = 
    [func](int key, int x, int y){func(key,x,y);};
}

void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
  GLFW_GLUT::g_keyboard_callback = 
    [func](unsigned char key, int x, int y){func(key,x,y);};
}

void glutMouseFunc(void (*func)(int button, int state, int x, int y))
{
  GLFW_GLUT::g_mouse_callback = 
    [func](int button, int state, int x, int y){func(button,state,x,y);};
}

void glutMotionFunc(void (*func)(int x, int y))
{
  GLFW_GLUT::g_motion_callback = [func](int x, int y){func(x,y);};
}

void glutPassiveMotionFunc(void (*func)(int x, int y))
{
  GLFW_GLUT::g_passive_motion_callback = [func](int x, int y){func(x,y);};
}

int glutGetModifiers()
{
  using namespace GLFW_GLUT;
  int mod = 0;
  mod |= g_mod & GLFW_MOD_SHIFT ? GLUT_ACTIVE_SHIFT : 0;
  mod |= g_mod & GLFW_MOD_ALT ? GLUT_ACTIVE_ALT : 0;
  mod |= g_mod & GLFW_MOD_CONTROL ? GLUT_ACTIVE_CTRL : 0;
  mod |= g_mod & GLFW_MOD_SUPER ? GLUT_ACTIVE_COMMAND : 0;
  return mod;
}

void glutSetCursor(int cursor)
{
  // Do nothing for now...
}

//void glutStrokeCharacter(void *font, int character)
//{
//  // Do nothing for now...
//}

//int glutStrokeWidth(void *font, int c)
//{
//  // Do nothing for now...
//}

