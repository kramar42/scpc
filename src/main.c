#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>      // for M_PI
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "main.h"
#include "plat.h"
#include "gl.h"
#include "ps.h"
#include "ga2.h"
#include "ga3.h"

#define TRIANGLE_SIZE 0.005

double sum(double acc, double val)
{
  return acc + val;
}

// a torus is now the product of two circles.
GA3p ga_torus(GA3 r, float s, float t, float r1, GA3 l1, float r2, GA3 l2)
{
  GA3 a = {0}, b = {0};
  return ga3_mul(r,
    ga3_circle(a,
      l2, r2, s),
    ga3_circle(b,
      l1, r1, t));
}

// and to sample its points we simply sandwich the origin ..
GA3p ga_point_on_torus(GA3 r, float s, float t)
{
  GA3 to = {0}, l1 = {0}, l2 = {0};
  ga_torus(to, s, t, 0.25f,
    ga3_mul(l1,
      ga3_e1,
      ga3_e2),
    0.6f,
    ga3_mul(l2,
      ga3_e1, ga3_e3));
  return ga3_transform(r, to, ga3_e123);
}

float* draw_triangle(float* vertices, GA2 center, GA2 dir)
{
  GA2 rotor    = {0}; ga2_rotor(rotor, center, (float) M_PI / 3.f);
  GA2 half_dir = {0}; ga2_muls(half_dir, dir, 0.5f);

  GA2 p1 = {0}, p2 = {0}, p3 = {0};

  if (ga2_distance(center, dir) < TRIANGLE_SIZE)
  {
    ga2_add      (p1, center, dir);
    ga2_add      (p1, p1, half_dir);
    ga2_transform(p2, rotor,  p1);
    ga2_transform(p3, rotor,  p2);

    arrput(vertices, -p1[5] / self.client.aspect);
    arrput(vertices,  p1[4]);
    arrput(vertices, -p2[5] / self.client.aspect);
    arrput(vertices,  p2[4]);
    arrput(vertices, -p3[5] / self.client.aspect);
    arrput(vertices,  p3[4]);
    return vertices;
  }

  ga2_add      (p1, center, dir);
  ga2_transform(p2, rotor,  p1);
  ga2_transform(p3, rotor,  p2);

  vertices = draw_triangle(vertices, p1, half_dir);
  vertices = draw_triangle(vertices, p2, half_dir);
  vertices = draw_triangle(vertices, p3, half_dir);
  return vertices;
}

int main()
{
  self = (Mind) {
    .client = {
      .aspect     = 16.0f / 9,
      .width      = 1920 * 1,
      .height     = 1080 * 1,
      .fullscreen = false,
      .vsync      = false,
    },
    .cursor = {
      .scroll = 0.005,
      .scale  = 0.01,
    }
  };

  Channel* fps = new_chan(100);
  GLFWwindow* window = gl_init();

  // vertex data
  uint32_t VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // vertex layout
  uint32_t VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // 2 floats per fertex, layout (location=0)
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // shaders
  uint32_t shader_program = gl_program("shaders/translate.vs", "shaders/position.fs");
  glUseProgram(shader_program);

  // wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  GA2 center = {0}; ga2_point(center, 0.0f, 0.0f);
  GA2 cursor = {0}, dir = {0};

  float* vertices = NULL;
  // loop
  for (;;)
  {
    float cursor_x = (float) self.cursor.x / self.client.width  * 2 - 1;
    float cursor_y = (float)-self.cursor.y / self.client.height * 2 + 1;

    // TODO fix aspect with trasformation matrix?
    ga2_point(cursor, cursor_x * self.client.aspect, cursor_y);
    ga2_sub  (dir, cursor, center);

    arrfree(vertices);
    vertices = draw_triangle(vertices, center, dir);

    // draw
    glClear(GL_COLOR_BUFFER_BIT);
    glBufferData(GL_ARRAY_BUFFER, arrlen(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, (int)arrlen(vertices) / 6);

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
