#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>      // for M_PI
#include <stdbool.h>
#ifdef _WIN32
  #include <windows.h> // for getcwd
  #include <conio.h>   // for getch
#else
  #include <unistd.h>  // for getcwd
#endif
#include <string.h>    // for memcpy

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "main.h"
#include "ga.h"
#include "gl.h"

typedef struct {
  size_t  capacity;
  size_t  current;
  bool    full;
  double* data;
} Channel;

Channel* new_chan(size_t capacity)
{
  Channel* c  = malloc(sizeof(Channel));
  c->capacity = capacity;
  c->current  = 0;
  c->full     = false;
  c->data     = (double*)malloc(capacity * sizeof(double));
  memset(c->data, 0, capacity * sizeof(double));
  printf("chan: %p, offset: %p, end: %p\n", c, c->data, c->data + capacity);
  return c;
}

void put_chan(Channel* chan, double dataptr)
{
  chan->data[chan->current++] = dataptr;
  if (chan->current == chan->capacity)
  {
    chan->full    = true;
    chan->current = 0;
  }
}

double sum(double acc, double val)
{
  return acc + val;
}

size_t chan_size(Channel* chan)
{
  return chan->full ? chan->capacity : chan->current;
}

double reduce_chan(Channel* chan, double(*reducer)(double,double))
{
  double result = 0;
  for (size_t i = 0; i < chan_size(chan); i++)
    result = reducer(result, chan->data[i]);
  return result;
}

int main()
{
  self = (Mind) {
    .client = {
      .aspect     = 16.0f / 9,
      .width      = 1920 * 2,
      .height     = 1080 * 2,
      .fullscreen = true,
      .last_frame = 0,
      .avg_fps    = 0,
    },
    .cursor       = { 0 }
  };

  Channel* fps = new_chan(100);

  GLFWwindow* window = gl_init();
  // glfwSetWindowPos(window, 10, 100);

  // vertex data
  uint32_t VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // vertex layout
  uint32_t VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // 3 floats per fertex
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  // layout (location=0)
  glEnableVertexAttribArray(0);

  // shaders
  uint32_t shader_program = gl_program("shaders/translate.vs", "shaders/position.fs");
  glUseProgram(shader_program);

  // wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  PGA2D center = {0}; ga_point(center, 0.0f, 0.0f);
  PGA2D rotor  = {0}; ga_rotor(rotor, center, (float) M_PI / 3.f);

  PGA2D cursor = {0}, second = {0}, third = {0};
  // loop
  for (;;)
  {
    float cursor_x =  (float) self.cursor.x / self.client.width  * 2 - 1;
    float cursor_y = -(float) self.cursor.y / self.client.height * 2 + 1;
    float aspect   = self.client.aspect;
    ga_point(cursor, cursor_x * aspect, cursor_y);
    ga_transform(second, rotor, cursor);
    ga_transform(third, rotor, second);

    float vertices[] = {
      -cursor[5] / aspect, cursor[4],
      -second[5] / aspect, second[4],
       -third[5] / aspect,  third[4]
    };
    // printf("x: %f, y: %f, aspect: %f\n", vertices[0], vertices[1], aspect);

    // draw
    glClear(GL_COLOR_BUFFER_BIT);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // update
    glfwSwapBuffers(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window)) break;

    // fps
    double time = glfwGetTime();
    double avg_fps = 1.0 / (time - self.client.last_frame);
    put_chan(fps, avg_fps);

    self.client.last_frame = time;
    self.client.avg_fps = reduce_chan(fps, sum) / chan_size(fps);
    printf("fps: %f\n", self.client.avg_fps);
  }

  success();
}

void success()
{
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void error(const char* message)
{
  printf("error: %s\n", message);
  glfwTerminate();
  char cwd[256];
#ifdef _WIN32
  GetCurrentDirectory(sizeof(cwd), cwd);
#else
  getcwd(cwd, sizeof(cwd));
#endif
  printf("cwd: %s\nPress any key...", cwd);
#ifdef _WIN32
  _getch();
#else
  getchar();
#endif
  exit(EXIT_FAILURE);
}
