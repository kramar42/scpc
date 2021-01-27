#include "gl.h"
#include "main.h"
#include "plat.h"
#include <stdio.h>
#include <stb_ds.h>

static void error_callback(int code, const char* description);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void cursor_callback(GLFWwindow* window, double x, double y);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void gl_check_shader(uint32_t sid);
static void gl_check_program(uint32_t pid);
static uint32_t gl_shader(const char* shader_filename, uint32_t shader_type);

float sum(float acc, float val)
{
  return acc + val;
}

void gl_init(GL* gl)
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
  if (self.window.fullscreen) gl->monitor = glfwGetPrimaryMonitor();

  // window
  gl->window = glfwCreateWindow(self.window.width, self.window.height, self.window.title, monitor, NULL);
  if (!gl->window) error("Failed to open GLFW window\n");

  // mouse
  glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(gl->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  // callbacks
  glfwSetFramebufferSizeCallback(gl->window, framebuffer_size_callback);
  glfwSetCursorPosCallback(gl->window, cursor_callback);
  glfwSetScrollCallback(gl->window, scroll_callback);

  // load gl
  glfwMakeContextCurrent(gl->window);
  gladLoadGL();
  if (self.window.vsync) glfwSwapInterval(1);
  else                   glfwSwapInterval(0);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  if (self.client.fps) gl->fps = new_chan(100);

  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version  = glGetString(GL_VERSION);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL Version: %s\n\n", version);

  glEnable(GL_DEPTH_TEST);

  gl->scenes = NULL;
  gl->fps    = NULL;
}

void gl_add_scene(GL* gl, Scene* scene)
{
  arrput(gl->scenes, scene);
}

bool gl_running(GL* gl)
{
  return !glfwWindowShouldClose(gl->window);
}

void update_camera(GL* gl)
{
  if (glfwGetKey(gl->window, GLFW_KEY_X))
    glfwSetWindowShouldClose(gl->window, GLFW_TRUE);

  if (glfwGetKey(gl->window, GLFW_KEY_E))
    self.camera.z += self.camera.speed;
  if (glfwGetKey(gl->window, GLFW_KEY_D))
    self.camera.z -= self.camera.speed;
  if (glfwGetKey(gl->window, GLFW_KEY_F))
    self.camera.x += self.camera.speed;
  if (glfwGetKey(gl->window, GLFW_KEY_S))
    self.camera.x -= self.camera.speed;
}

void gl_update(GL* gl)
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  if (gl->scenes)
    for (size_t i = 0; i < arrlenu(gl->scenes); i++)
      scene_render(gl->scenes[i]);

  glfwSwapBuffers(gl->window);
  glfwPollEvents();
  update_camera(gl);

  // fps
  float time = (float)glfwGetTime();
  if (gl->fps)
  {
    float avg_fps = 1.0f / (time - self.client.last_frame);
    put_chan(gl->fps, avg_fps);
    self.client.last_frame = time;
    self.client.avg_fps = reduce_chan(gl->fps, sum) / chan_size(gl->fps);
  }

  float dtick = time - self.client.last_tick;
  if (dtick > self.client.tick)
  {
    self.client.last_tick = time;
    if (gl->fps)
      printf("fps: %f\n", self.client.avg_fps);
    if (gl->scenes)
    {
      size_t indices = 0;
      for (size_t i = 0; i < arrlenu(gl->scenes); i++)
        indices += arrlenu(gl->scenes[i]->indices);
      printf("drawing %zd triangles\n", indices / 3); //, indices * sizeof(float) / 1024.f / 1024.f);
    }
  }
}

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

// STATICS

static void error_callback(int code, const char* description)
{
  (void)code;
  error(description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  (void)window;
  glViewport(0, 0, width, height);

  self.window.width = width;
  self.window.height = height;
  self.window.aspect = ((float) width) / height;
}

static void cursor_callback(GLFWwindow* window, double x, double y)
{
  (void)window;
  self.cursor.x = (int)x;
  self.cursor.y = (int)y;
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  (void)window; (void)xoffset; (void)yoffset;
  self.cursor.scroll += (float)yoffset * self.cursor.scale;
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