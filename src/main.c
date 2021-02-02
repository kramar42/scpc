#include "impls.h"
#include "main.h"
#include "gl.h"
#include "scene.h"

#define XY_ASPECT     (16.0f / 9.0f)
#define XZ_ASPECT     (16.0f / 9.0f)
#define SIZE          (1920 * 1)
#define arrsize(ARR)  (sizeof(ARR)/sizeof(ARR[0]))

void draw_axis(Scene* scene)
{
  float scale = 100;

  float vertices[] = {
    // xyz               // color
        0,     0,     0, // 0 ,   0,   0,
    scale,     0,     0, //255,   0,   0,
        0, scale,     0, //  0, 255,   0,
        0,     0, scale, //  0,   0, 255,
  };

  int vertex_size = 3;

  uint32_t offset = (uint32_t)arrlen(scene->vertices) / vertex_size;
  uint32_t indices[] = {
    0, 1, 0, 2, 0, 3,
  };

  for (size_t i = 0; i < arrsize(vertices); i++)
    arrput(scene->vertices, vertices[i]);

  for (size_t i = 0; i < arrsize(indices); i++)
    arrput(scene->indices, indices[i] + offset);
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

  for (size_t i = 0; i < arrsize(vertices); i++)
    arrput(scene->vertices, vertices[i]);

  for (size_t i = 0; i < arrsize(indices); i++)
    arrput(scene->indices, indices[i] + offset);
}

int main()
{
  GA3 p, f, r, u;

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
      .position    = ga3_point(p, 0.5f, 1.5f, -4.5f),
      .forward     = ga3_id   (f, ga3_e021),
      .right       = ga3_id   (r, ga3_e032),
      .up          = ga3_id   (u, ga3_e013),
      .speed       = 0.03f,
      .sensitivity = 0.001f,
    },
    .cursor = {
      .scale       = 1.0f,
    }
  };

  GL gl; gl_init(&gl);

  Scene axis = (Scene) {
    .vs_shader      = "shaders/cube.vs.glsl",
    .fs_shader      = "shaders/position.fs.glsl",
    .primitive_type = GL_LINES,
  };
  scene_init(&axis);
  gl_add_scene(&gl, &axis);

  Scene debug = (Scene) {
    .vs_shader      = "shaders/cube.vs.glsl",
    .fs_shader      = "shaders/position.fs.glsl",
  };
  scene_init(&debug);
  gl_add_scene(&gl, &debug);

  while (gl_running(&gl))
  {
    gl_clear(&gl);
    gl_pool(&gl);

    // [-1; +1]
    // float cursor_x = -(float)self.cursor.last_x / self.window.width  * 2 + 1;
    // float cursor_y = -(float)self.cursor.last_y / self.window.height * 2 + 1;
    // float scroll   =  (float)self.cursor.scroll;

    draw_axis(&axis);

    // draw cubes
    {
      GA3 target;    ga3_point(target,  5.0f, 0.0f,  5.0f);
      GA3 left;      ga3_point(left,   -5.0f, 0.5f, -5.0f);
      GA3 direction; ga3_add  (direction, ga3_e123, self.camera.forward);

      // cube at origin
      draw_cube(&debug, (GA3p)ga3_e123, 1.0f);
      // cube shifted right and forward
      draw_cube(&debug, target,  1.0f);
      // cube that rotates around origin in a direction camera is facing
      draw_cube(&debug, direction, 0.3f);
      // directions of cubes
      for (size_t i = 0; i < 10; i++)
      {
        ga3_add(left, left, self.camera.forward);
        draw_cube(&debug, left, 0.3f);
      }
      ga3_point(left,  -5.0f, 0.5f, -5.0f);
      for (size_t i = 0; i < 10; i++)
      {
        ga3_add(left, left, self.camera.up);
        draw_cube(&debug, left, 0.3f);
      }
      ga3_point(left,  -5.0f, 0.5f, -5.0f);
      for (size_t i = 0; i < 10; i++)
      {
        ga3_add(left, left, self.camera.right);
        draw_cube(&debug, left, 0.3f);
      }
    }

    // position translation
    GA3 dir_l; ga3_join(dir_l, ga3_e123, self.camera.position);
    GA3 dir_il; ga3_mul(dir_il, dir_l, ga3_e0123);
    float dist_p = ga3_distance(ga3_e123, self.camera.position);
    GA3 trans_p; ga3_translator(trans_p, dir_il, dist_p);

    // rotation that moves Z axis (e12) to be `forward` direction
    GA3 rot_line;  ga3_join(rot_line,  ga3_e123,  self.camera.forward);
    GA3 rot_plane; ga3_join(rot_plane, ga3_e021, rot_line); //,  ga3_e021);
    GA3 rot_axis;  ga3_dot (rot_axis,  rot_plane, ga3_e123);

    float rot_angle = ga3_angle(rot_line, ga3_e12);
    GA3 rot_forward;  ga3_rotor(rot_forward, rot_axis, rot_angle / 2);

#ifdef DEBUG
    printf("angle is %f, %f\n", rot_angle, rot_angle * 180.0f / M_PI);
#endif

    // fix roll of `up` direction
    GA3 new_up; ga3_transform(new_up, rot_forward, ga3_e31);
    GA3 up_dir; ga3_join     (up_dir, ga3_e123, self.camera.up);
    rot_angle = ga3_angle    (new_up, up_dir);
    GA3 rot_up; ga3_rotor    (rot_up, self.camera.forward, rot_angle / 2);
    // printf("angle is %f, %f\n", rot_angle, rot_angle * 180.0f / M_PI);

    // transformations are in order
    GA3 trans;
    const GA3* transl[] = {
      &trans_p,
      &rot_forward,
      // &rot_up,
    };
    ga3_combine(trans, transl, sizeof(transl) / sizeof(transl[0]));

    // setup scene
    scene_umat4 (&axis,  "trans",  trans);
    scene_umat4 (&debug, "trans",  trans);
    // render
    gl_update(&gl);
  }

  success();
}
