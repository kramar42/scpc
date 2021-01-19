#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>      // for M_PI
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "main.h"
#include "util.h"
#include "gl.h"
#include "ps.h"
#include "pga2d.h"

double sum(double acc, double val)
{
  return acc + val;
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

  GA2 center = {0}; ga2_point(center, 0.0f, 0.0f);
  GA2 rotor  = {0}; ga2_rotor(rotor, center, (float) M_PI / 3.f);

  GA2 cursor = {0}, second = {0}, third = {0};
  // loop
  for (;;)
  {
    float cursor_x =  (float) self.cursor.x / self.client.width  * 2 - 1;
    float cursor_y = -(float) self.cursor.y / self.client.height * 2 + 1;
    float aspect   = self.client.aspect;
    ga2_point(cursor, cursor_x * aspect, cursor_y);
    ga2_transform(second, rotor, cursor);
    ga2_transform(third, rotor, second);

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
