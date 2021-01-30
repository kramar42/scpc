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
    x - h, y - h, z + h, // lbn
    x - h, y - h, z - h, // lbf
    x + h, y - h, z - h, // rbf
    x + h, y - h, z + h, // rbn
    x + h, y + h, z + h, // rtn
    x + h, y + h, z - h, // rtf
    x - h, y + h, z - h, // ltf
    x - h, y + h, z + h, // ltn
  };

  uint32_t offset = (uint32_t)arrlen(scene->vertices) / 3;
  uint32_t indices[] = {
    0, 1, 2, 2, 3, 0,    // bottom (b)
    0, 7, 6, 6, 1, 0,    // left   (l)
    1, 6, 5, 5, 2, 1,    // back   (f)
    4, 5, 2, 2, 3, 4,    // right  (r)
    0, 7, 4, 4, 3, 0,    // front  (n)
    7, 6, 5, 5, 4, 7,    // top    (t)
  };

  for (size_t i = 0; i < 3 * 8; i++)
    arrput(scene->vertices, vertices[i]);

  for (size_t i = 0; i < 3 * 2 * 6; i++)
    arrput(scene->indices, indices[i] + offset);
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
  GA3 position, forward, right, up;
  ga3_id(position, ga3_e123);
  ga3_id(forward, ga3_e021);
  ga3_id(right, ga3_e032);
  ga3_id(up, ga3_e013);
  // ga3_add(position, position, ga3_e021);

  self = (Mind) {
    .window = {
      .title       = "SCPC",
      .aspect      = XY_ASPECT,
      .width       = SIZE,
      .height      = (uint32_t)(SIZE / XY_ASPECT),
      .depth       = (uint32_t)(SIZE / XZ_ASPECT),
      .front       = 0.1f,
      .fullscreen  = false,
      .vsync       = true,
    },
    .stats = {
      .fps         = false,
      .tick        = 2,
    },
    .camera = {
      .position    = position,
      .forward     = forward,
      .right       = right,
      .up          = up,
      .speed       = 0.03f,
      .sensitivity = 0.0005f,
    },
    .cursor = {
      .scale       = 1.0f,
    }
  };

  GL gl; gl_init(&gl);

  /*
  Scene cube = (Scene) {
    .vs_shader    = "shaders/cube.vs.glsl",
    .fs_shader    = "shaders/position.fs.glsl",
    .polygon_mode = GL_FILL,
  };
  scene_init(&cube);
  scene_perspective(&cube, 500.0f);
  gl_add_scene(&gl, &cube);
  */

  Scene debug = (Scene) {
    .vs_shader    = "shaders/cube.vs.glsl",
    .fs_shader    = "shaders/position.fs.glsl",
    .polygon_mode = GL_LINE,
  };
  scene_init(&debug);
  scene_perspective(&debug, 2000.0f);
  gl_add_scene(&gl, &debug);

  // done with init. define some elements
  GA2 origin2; ga2_point(origin2, 0.0f, 0.0f);
  GA3 origin3; ga3_point(origin3, 0.0f, 0.0f, 0.0f);
  GA3 center;  ga3_point(center,  5.0f, 0.0f, 5.0f);

  while (gl_running(&gl))
  {
    gl_clear(&gl);

    // [-1; +1]
    // float cursor_x = -(float)self.cursor.x / self.window.width  * 2 + 1;
    // float cursor_y = -(float)self.cursor.y / self.window.height * 2 + 1;
    // float scroll   =  (float)self.cursor.scroll;

#if 1
    draw_cube(&debug, origin3, 1.0f);
    draw_cube(&debug, center,  1.0f);
    // cube that rotates around origin in a direction camera is facing
    GA3 direction; ga3_add(direction, origin3, self.camera.forward);
    draw_cube(&debug, direction, 0.3f);
    // line of cubes to test perspective
    GA3 center; ga3_point(center, -5.0f, 0.5f, -5.0f);
    for (size_t i = 0; i < 10; i++)
    {
      ga3_add(center, center, self.camera.forward);
      draw_cube(&debug, center, 0.3f);
    }

    // GA3 trans_x, trans_y, trans_z;
    // ga3_translator(trans_x, ga3_e01, self.camera.x);
    // ga3_translator(trans_y, ga3_e02, self.camera.y);
    // ga3_translator(trans_z, ga3_e03, self.camera.z);

    // position translation
    GA3 dir_p; ga3_join(dir_p, origin3, self.camera.position);
    GA3 dir_pi; ga3_mul(dir_pi, dir_p, ga3_e0123);
    float dist_p = ga3_distance(origin3, self.camera.position);
    GA3 trans_p; ga3_translator(trans_p, dir_pi, dist_p);

    // forward is down Z axis
    GA3 rot_line, rot_plane, rot_axis, rot_forward;
    ga3_join(rot_line, origin3, self.camera.forward);
    float rot_angle = ga3_angle(rot_line, ga3_e12) / 2;
    // printf("angle is %f, %f\n", rot_angle, rot_angle * 180.0f / M_PI);
    ga3_join(rot_plane, rot_line, ga3_e021);
    // that's fun...
    // ga3_dot(rot_axis, rot_line, origin3);
    ga3_dot(rot_axis, rot_plane, origin3);
    ga3_rotor(rot_forward, rot_axis, rot_angle);
    // ga3_rotor(rot_y, ga3_e23,  cursor_y);

    // transformations are in order
    GA3 trans;
    const GA3* transl[] = {
      // &rot_y,
      // &rot_z,
      &trans_p,
      &rot_forward,
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
    // scene_umat4 (&cube, "trans",  trans);
    scene_umat4 (&debug, "trans",  trans);
    // render
    gl_update(&gl);
  }

  success();
}
