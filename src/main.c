#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

#define _ps_impl_
#include "ps.h"
#define _ga2_impl_
#include "ga2.h"
#define _ga3_impl_
#include "ga3.h"

#include "main.h"
#include "plat.h"
#include "gl.h"
#include "scene.h"

#define TRIANGLE_SIZE 0.005

float* draw_triangle(float* vertices, GA2 center, GA2 dir)
{
  GA2 rotor, half_dir;
  ga2_rotor(rotor, center, (float) M_PI / 3.f);
  ga2_smul(half_dir, 0.5f, dir);

  GA2 p1, p2, p3;

  if (ga2_distance(center, dir) < TRIANGLE_SIZE)
  {
    // printf("sanity check: %f %f\n", -center[5], center[4]);
    ga2_add      (p1, center, dir);
    ga2_add      (p1, p1, half_dir);
    ga2_transform(p2, rotor,  p1);
    ga2_transform(p3, rotor,  p2);

    arrput(vertices, -p1[5]);
    arrput(vertices,  p1[4]);
    arrput(vertices,  1.0f);
    arrput(vertices, -p2[5]);
    arrput(vertices,  p2[4]);
    arrput(vertices,  1.0f);
    arrput(vertices, -p3[5]);
    arrput(vertices,  p3[4]);
    arrput(vertices,  1.0f);
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

float* draw_square(float* v, GA3 point, float depth, float size)
{
  float x = point[13], y = point[12], z = point[11], h = size / 2;
  // printf("sanity check: %f %f %f %f %f\n", x, y, z, depth, h);

  arrput(v, x - h); arrput(v, y - h); arrput(v, z);
  arrput(v, x - h); arrput(v, y + h); arrput(v, z - depth);
  arrput(v, x + h); arrput(v, y + h); arrput(v, z - depth);

  arrput(v, x - h); arrput(v, y - h); arrput(v, z);
  arrput(v, x + h); arrput(v, y + h); arrput(v, z - depth);
  arrput(v, x + h); arrput(v, y - h); arrput(v, z);

  return v;
}

// сделать углы фрустума подвижными. они напрямую привязаны к текущей проекции
// чтобы понимать что происходит - в углу привычная проекция показывает фрустум
float* draw_frustum(float* v, GA3 line, float* params)
{
  (void)v;(void)line;(void)params;
  return v;
}

// left mouse click - select
// left mouse drag - drag the world
// right mouse click - meta
// right mouse drag - perspective
// middle click - toggle perspective lock
int main()
{
  self = (Mind) {
    .window = {
      .title      = "SCPC",
      .aspect     = 16.0f / 9,
      .width      = 1920 * 1,
      .height     = 1080 * 1,
      .fullscreen = false,
      .vsync      = true,
    },
    .client = {
      .tick       = 1,
    },
    .camera = {
      .speed      = 0.05f,
    },
    .cursor = {
      .scale      = 0.37f,
    }
  };

  GL gl; gl_init(&gl);

  Scene debug = (Scene) {
    .vs_shader = "shaders/default.vs.glsl",
    .fs_shader = "shaders/position.fs.glsl",
  };
  scene_init(&debug);
  gl_add_scene(&gl, &debug);

  // done with init. define some elements
  GA3 origin3; ga3_point(origin3, 0.0f, 0.0f, 0.0f);
  GA2 origin2; ga2_point(origin2, 0.0f, 0.0f);

  GA3 x_axis, y_axis, z_axis;
  ga3_sadd(x_axis, 0, ga3_e12);
  ga3_sadd(y_axis, 0, ga3_e23);
  ga3_sadd(z_axis, 0, ga3_e31);

  float* vertices = NULL;
  while (gl_running(&gl))
  {
    arrfree(vertices);

    // [-1; +1]
    float cursor_x =  (float)self.cursor.x / self.window.width  * 2 - 1;
    float cursor_y = -(float)self.cursor.y / self.window.height * 2 + 1;
    float scroll   =  (float)self.cursor.scroll;

    // generate shapes
#if 1
    // transformed square
    GA3 transformation, trans_x, trans_y, center, point;
    ga3_translator(trans_x, x_axis, cursor_x);
    ga3_translator(trans_y, y_axis, cursor_y);
    ga3_mul(transformation, trans_x, trans_y);

    ga3_point(center, 0.0f, 0.0f, 0.0f);
    ga3_transform(point, transformation, center);

    vertices = draw_square(vertices, center, scroll * self.cursor.scale, 0.5f);
#endif

#if 0
    // sierpinski
    GA2 cursor, dir;
    ga2_point(cursor, cursor_x * self.client.aspect, cursor_y);
    ga2_sub  (dir, cursor, origin2);

    vertices = draw_triangle(vertices, origin2, dir);
#endif

    // setup scene
    scene_send_vertices(&debug, vertices);
    scene_umat4(&debug, "trans", transformation);
    // render
    gl_update(&gl);
  }

  success();
}
