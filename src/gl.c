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

  gl->monitor = NULL;
  if (self.window.fullscreen) gl->monitor = glfwGetPrimaryMonitor();

  // window
  gl->window = glfwCreateWindow(self.window.width, self.window.height, self.window.title, gl->monitor, NULL);
  if (!gl->window) error("Failed to open GLFW window\n");

  // mouse
  glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(gl->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  // callbacks
  glfwSetFramebufferSizeCallback(gl->window, framebuffer_size_callback);
  glfwSetScrollCallback(gl->window, scroll_callback);

  // load gl
  glfwMakeContextCurrent(gl->window);
  gladLoadGL();
  if (self.window.vsync) glfwSwapInterval(1);
  else                   glfwSwapInterval(0);

  glfwSetInputMode(gl->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version  = glGetString(GL_VERSION);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL Version: %s\n\n", version);

  glEnable(GL_DEPTH_TEST);

  gl->scenes = NULL;
  gl->fps    = NULL;
  if (self.stats.fps) gl->fps = new_chan(100);
}

void gl_add_scene(GL* gl, Scene* scene)
{
  arrput(gl->scenes, scene);
}

bool gl_running(GL* gl)
{
  return !glfwWindowShouldClose(gl->window);
}

void gl_clear(GL* gl)
{
  for (size_t i = 0; i < arrlenu(gl->scenes); i++)
    scene_clear(gl->scenes[i]);
}

/* left mouse click - select
   left mouse drag - drag the world
   right mouse click - meta
   right mouse drag - perspective
   middle click - toggle perspective lock */
void update_camera(GL* gl)
{
  if (glfwGetKey(gl->window, GLFW_KEY_X))
    glfwSetWindowShouldClose(gl->window, GLFW_TRUE);

  // WASD
  GA3 tmp, delta = {0};
  if (glfwGetKey(gl->window, GLFW_KEY_E))
  {
    ga3_smul(tmp,  1, self.camera.forward);
    ga3_add(delta, delta, tmp);
  }
  if (glfwGetKey(gl->window, GLFW_KEY_D))
  {
    ga3_smul(tmp, -1, self.camera.forward);
    ga3_add(delta, delta, tmp);
  }
  if (glfwGetKey(gl->window, GLFW_KEY_F))
  {
    ga3_smul(tmp,  1, self.camera.right);
    ga3_add(delta, delta, tmp);
  }
  if (glfwGetKey(gl->window, GLFW_KEY_S))
  {
    ga3_smul(tmp, -1, self.camera.right);
    ga3_add(delta, delta, tmp);
  }

  // todo multiply with last_fps
  ga3_smul(delta, self.camera.speed, delta);
  ga3_add(self.camera.position, self.camera.position, delta);
  // ga3_print(self.camera.position);

  // mouse rotations
  double xpos, ypos;
  glfwGetCursorPos(gl->window, &xpos, &ypos);
  float dx = (float)xpos - self.cursor.last_x,
        dy = (float)ypos - self.cursor.last_y;
  self.cursor.last_x = (float)xpos;
  self.cursor.last_y = (float)ypos;

  GA3 axis_up, axis_right, rot_x, rot_y;
  ga3_join(axis_up,    ga3_e123,   self.camera.up);
  ga3_join(axis_right, ga3_e123,   self.camera.right);
  ga3_rotor(rot_x,     axis_up,    dx * self.camera.sensitivity);
  ga3_rotor(rot_y,     axis_right, dy * self.camera.sensitivity);

  // rotate around up
  if (fabs(dx) > 0.1)
  {
    // printf("time to rotate!\n");
    ga3_transform(self.camera.forward, rot_x, self.camera.forward);
    ga3_transform(self.camera.right,   rot_x, self.camera.right);
  }
  // rotate around right
  if (fabs(dy) > 0.1)
  {
    // ga3_transform(self.camera.forward, rot_y, self.camera.forward);
    // ga3_transform(self.camera.up,      rot_y, self.camera.up);
  }

  int state = glfwGetMouseButton(gl->window, GLFW_MOUSE_BUTTON_LEFT);
  if (state == GLFW_PRESS)
  {
  }
}

void gl_update(GL* gl)
{
  glfwPollEvents();
  update_camera(gl);

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  if (gl->scenes)
    for (size_t i = 0; i < arrlenu(gl->scenes); i++)
      scene_render(gl->scenes[i]);

  glfwSwapBuffers(gl->window);

  // stats: fps
  float time = (float)glfwGetTime();
  if (gl->fps)
  {
    float last_fps = 1.0f / (time - self.stats.last_frame);
    put_chan(gl->fps, last_fps);
    self.stats.last_fps   = last_fps;
    self.stats.last_frame = time;
    self.stats.avg_fps    = reduce_chan(gl->fps, sum) / chan_size(gl->fps);
  }

  // print stats on tick
  float dtick = time - self.stats.last_tick;
  if (dtick > self.stats.tick)
  {
    self.stats.last_tick = time;
    if (gl->fps)
      printf("fps: %f\n", self.stats.avg_fps);
    if (gl->scenes)
    {
      size_t indices = 0;
      for (size_t i = 0; i < arrlenu(gl->scenes); i++)
        indices += arrlenu(gl->scenes[i]->indices);
      // assume all scenes were drawing triangles
      printf("drawing %zd vertices\n", indices); //, indices * sizeof(float) / 1024.f / 1024.f);
    }
#ifdef DEBUG
    // printf("camera.x = %f, camera.y = %f, camera.z = %f\n", self.camera.x, self.camera.y, self.camera.z);
    // printf("cursor.x: %f, cursor.y: %f\n", self.cursor.x, self.cursor.y);
#endif
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

// CALLBACKS

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

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  (void)window; (void)xoffset; (void)yoffset;
  self.cursor.scroll += (float)yoffset * self.cursor.scale;
}

// HELPERS

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
