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

  // setup
  GLFWwindow* window = gl_init();
  glClearColor(0.4f, 0.3f, 0.4f, 0.0f);

  // loop
  for (;;)
  {
    glClear(GL_COLOR_BUFFER_BIT);
    gl_point(self.cursor.x, self.cursor.y);
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwWindowShouldClose(window)) break;
  }

  success();
}