#ifndef _scene_h_
#define _scene_h_

#include <stdint.h>

typedef struct {
  const char* vs_shader;
  const char* fs_shader;

  uint32_t    vbo;
  uint32_t    vao;
  uint32_t    shader;

  float*      vertices;
} Scene;

void scene_init          (Scene* scene);
void scene_send_vertices (Scene* scene, float* vertices);
void scene_render        (Scene* scene);
void scene_ufloat        (Scene* scene, const char* uniform, float value);
void scene_u2float       (Scene* scene, const char* uniform, float first, float second);
void scene_umat4         (Scene* scene, const char* uniform, float value[16]);

#endif // _scene_h