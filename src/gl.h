#ifndef _gl_h_
#define _gl_h_

#include "main.h"

static void success()
{
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

static void error(const char* message)
{
  fprintf(stderr, "error: %s", message);
  glfwTerminate();
  exit(EXIT_FAILURE);
}

static void error_callback(int code, const char* description)
{
  (void)code;
  error(description);
}

static void gl_point(int x, int y)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 640.0, 480.0, 0.0, 0.0, 50.0);

  glBegin(GL_POINTS);
  glColor3f(1,1,1);
  glVertex2i(x, y);
  glEnd();
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  (void) window;
  glViewport(0, 0, width, height);

  PGA2D p = {0};
  ga_point(p, width / 2.0f, height / 2.0f);
  ga_print(p);
  pga2d_muls(p, p, 0.5f);
  printf("norm: %f\n", pga2d_norm(p));
  ga_print(p);
  printf("norm: %f\n", pga2d_norm(p));

  PGA2D r = {0};
  ga_rotor(r, p, (float) M_PI / 3.0f);
  ga_print(r);
  printf("norm: %f\n", pga2d_norm(r));

  printf("width: %d height: %d\n", width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  (void)scancode;
  (void)mods;
  if (key == GLFW_KEY_X && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_callback(GLFWwindow* window, double x, double y)
{
  (void)window;
  self.cursor.x = (int)x;
  self.cursor.y = (int)y;
  printf("x: %d, y: %d\n", self.cursor.x, self.cursor.y);
}

static GLFWwindow* gl_init()
{
  // init
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) error("Failed to initialize GLFW\n");

  // hints
  glfwWindowHint(GLFW_SAMPLES, 4);

  // window
  GLFWwindow* window = glfwCreateWindow(self.client.width, self.client.height, "SCPC", NULL, NULL);
  if (!window) error("Failed to open GLFW window\n");

  // mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  // callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_callback);

  // load gl
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  return window;
}

#endif _gl_h_