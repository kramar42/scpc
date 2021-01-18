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

  // init
  GLFWwindow* window = gl_init();
  glClearColor(0.4f, 0.3f, 0.4f, 0.0f);

  // triangle vertices
  uint32_t VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  float vertices[] = {
    // first triangle
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, 0.5f, 0.0f, // top left
    // second triangle
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f // top left
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // shaders
  uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  gl_check_shader(vertex_shader);

  uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  gl_check_shader(fragment_shader);

  // enable shader program
  uint32_t shader_program = gl_program(vertex_shader, fragment_shader);
  glUseProgram(shader_program);

  // vertex array object
  uint32_t VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // loop
  for (;;)
  {
    glClear(GL_COLOR_BUFFER_BIT);
    gl_point(self.cursor.x, self.cursor.y);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwWindowShouldClose(window)) break;
  }

  success();
}