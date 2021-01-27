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

#define XY_ASPECT     (16.0f / 9.0f)
#define XZ_ASPECT     (16.0f / 9.0f)
#define SIZE          (1920 * 1)
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

void draw_cube(Scene* scene, GA3 point, float size)
{
  float x = point[13], y = point[12], z = point[11], h = size / 2;
  // printf("sanity check: %f %f %f %f %f\n", x, y, z, depth, h);

  float vertices[] = {
    x - h, y - h, z + h,
    x - h, y - h, z - h,
    x + h, y - h, z - h,
    x + h, y - h, z + h,
    x + h, y + h, z + h,
    x + h, y + h, z - h,
    x - h, y + h, z - h,
    x - h, y + h, z + h,
  };

  uint32_t indices[] = {
    0, 1, 2, 2, 3, 0, // bottom
    0, 7, 6, 6, 1, 0, // left
    1, 6, 5, 5, 2, 1, // back
    4, 5, 2, 2, 3, 4, // right
    0, 7, 4, 4, 3, 0, // front
    7, 6, 5, 5, 4, 7, // top
  };

  for (size_t i = 0; i < 3 * 8; i++)
    arrput(scene->vertices, vertices[i]);

  for (size_t i = 0; i < 3 * 2 * 6; i++)
    arrput(scene->indices, indices[i]);
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
      .aspect     = XY_ASPECT,
      .width      = SIZE,
      .height     = (uint32_t)(SIZE / XY_ASPECT),
      .depth      = (uint32_t)(SIZE / XZ_ASPECT),
      .front      = 0.1f,
      .fullscreen = false,
      .vsync      = true,
    },
    .stats = {
      .fps        = false,
      .tick       = 2,
    },
    .camera = {
      .looking_at = (GA3p)ga3_e03,
      .speed      = 0.10f,
    },
    .cursor = {
      .scale      = 1.0f,
    }
  };

  GL gl; gl_init(&gl);

  Scene cube = (Scene) {
    .vs_shader    = "shaders/cube.vs.glsl",
    .fs_shader    = "shaders/position.fs.glsl",
    .polygon_mode = GL_LINE,
  };
  scene_init(&cube);
  scene_perspective(&cube, 500.0f);
  // gl_add_scene(&gl, &cube);

  Scene debug = (Scene) {
    .vs_shader    = "shaders/cube.vs.glsl",
    .fs_shader    = "shaders/position.fs.glsl",
    .polygon_mode = GL_LINE,
  };
  scene_init(&debug);
  scene_perspective(&debug, 500.0f);
  gl_add_scene(&gl, &debug);

  // done with init. define some elements
  GA3 origin3; ga3_point(origin3, 0.0f, 0.0f, 0.0f);
  GA2 origin2; ga2_point(origin2, 0.0f, 0.0f);

  while (gl_running(&gl))
  {
    gl_clear(&gl);

    // [-1; +1]
    float cursor_x = -(float)self.cursor.x / self.window.width  * 2 + 1;
    float cursor_y = -(float)self.cursor.y / self.window.height * 2 + 1;
    // float scroll   =  (float)self.cursor.scroll;
    GA3 center; ga3_point(center, 0.0f, 0.0f, 0.0f);

#if 1
    // draw cube
    draw_cube(&cube, center, 1.0f);
    // debug outline
    draw_cube(&debug, center, 1.1f);

    // WASD translations
    GA3 trans_x, trans_y, trans_z;
    ga3_translator(trans_x, ga3_e01, self.camera.x);
    ga3_translator(trans_y, ga3_e02, self.camera.y);
    ga3_translator(trans_z, ga3_e03, self.camera.z);
    // mouse rotations
    GA3 rot_x, rot_y;
    // rotaty around Y
    ga3_rotor(rot_x, ga3_e31,  cursor_x);
    ga3_rotor(rot_y, ga3_e23,  cursor_y);

    GA3 trans;
    // transformations in reverse order
    GA3* transl[] = {
      &rot_x,
      &trans_x, &trans_y, &trans_z,
      // &rot_y,
    };
    ga3_combine(trans, transl, sizeof(transl) / sizeof(transl[0]));
#endif

#if 0
    // sierpinski
    GA2 cursor, dir;
    ga2_point(cursor, cursor_x, cursor_y);
    ga2_sub  (dir, cursor, origin2);

    draw_triangle(&debug, origin2, dir);
#endif

    // setup scene
    scene_umat4 (&cube, "trans",  trans);
    scene_umat4 (&debug, "trans",  trans);
    // render
    gl_update(&gl);
  }

  success();
}
