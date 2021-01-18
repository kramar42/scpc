#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "main.h"
#include "ga.h"
#include "gl.h"

int main()
{
  self = (Mind) {
    .client = {
      .width  = 640,
      .height = 480
    },
    .cursor = {
      .x = 0,
      .y = 0
    }
  };

  float vertices[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };

  GLFWwindow* window = gl_init();

  // push vertex data
  uint32_t VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // how verteces are layed-out?
  uint32_t VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // 3 floats per fertex
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // layout (location=0)
  glEnableVertexAttribArray(0);

  // shaders
  uint32_t shader_program = gl_program("shaders/translate.vs", "shaders/position.fs");
  glUseProgram(shader_program);

  // wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // loop
  for (;;)
  {
    float cursor_x =  (float) self.cursor.x / self.client.width  - 0.5f;
    float cursor_y = -(float) self.cursor.y / self.client.height + 0.5f;
    // draw
    gl_set_u2float(shader_program, "translate", cursor_x, cursor_y);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // update
    glfwSwapBuffers(window);
    glfwPollEvents();
    if (glfwWindowShouldClose(window)) break;
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
  printf("error: %s", message);
  glfwTerminate();
  exit(EXIT_FAILURE);
}
