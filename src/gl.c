#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gl.h"
#include "util.h"

static void error_callback(int code, const char* description)
{
  (void)code;
  error(description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  (void)window;
  glViewport(0, 0, width, height);

  self.client.width = width;
  self.client.height = height;
  self.client.aspect = ((float) width) / height;
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
}

static void gl_check_shader(uint32_t sid)
{
  int code;
  glGetShaderiv(sid, GL_COMPILE_STATUS, &code);
  if (!code)
  {
    char log[512];
    glGetShaderInfoLog(sid, sizeof(log), NULL, log);
    printf("shader compilation failed\n");
    error(log);
  }
}

static void gl_check_program(uint32_t pid)
{
  int code;
  glGetProgramiv(pid, GL_LINK_STATUS, &code);
  if (!code)
  {
    char log[512];
    glGetProgramInfoLog(pid, sizeof(log), NULL, log);
    printf("program linkage failed\n");
    error(log);
  }
}

static uint32_t gl_shader(const char* shader_filename, uint32_t shader_type)
{
  size_t filesize;
  const char* shader_source = slurp_file(shader_filename, &filesize);

  uint32_t shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &shader_source, NULL);
  glCompileShader(shader);
  gl_check_shader(shader);
  free((void*)shader_source);
  return shader;
}

// public

uint32_t gl_program(const char* vertex_filename, const char* fragment_filename)
{
  uint32_t vertex_shader = gl_shader(vertex_filename, GL_VERTEX_SHADER);
  uint32_t fragment_shader = gl_shader(fragment_filename, GL_FRAGMENT_SHADER);

  uint32_t shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  gl_check_program(shader_program);
  return shader_program;
}

void gl_set_ufloat (uint32_t program, const char* uniform, float value)
{
  uint32_t uniform_loc = glGetUniformLocation(program, uniform);
  glUniform1f(uniform_loc, value);
}

void gl_set_u2float (uint32_t program, const char* uniform, float first, float second)
{
  uint32_t uniform_loc = glGetUniformLocation(program, uniform);
  glUniform2f(uniform_loc, first, second);
}

GLFWwindow* gl_init()
{
  // init
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) error("Failed to initialize GLFW\n");

  // hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWmonitor* monitor = NULL;
  if (self.client.fullscreen) monitor = glfwGetPrimaryMonitor();

  // window
  GLFWwindow* window = glfwCreateWindow(self.client.width, self.client.height, "SCPC", monitor, NULL);
  if (!window) error("Failed to open GLFW window\n");

  // mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  // callbacks
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_callback);

  // load gl
  glfwMakeContextCurrent(window);
  gladLoadGL();
  // glfwSwapInterval(1);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  return window;
}
