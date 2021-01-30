#ifndef _scene_h_
#define _scene_h_

#include <stdint.h>

typedef struct {
  const char* vs_shader;
  const char* fs_shader;
  uint32_t    polygon_mode;
  uint32_t    primitive_type;

  uint32_t    vao;
  uint32_t    vbo;
  uint32_t    ebo;
  uint32_t    shader;

  float*      vertices;
  uint32_t*   indices;
} Scene;

void scene_init        (Scene* scene);
void scene_clear       (Scene* scene);
void scene_render      (Scene* scene);
void scene_perspective (Scene* scene, const float focal);
void scene_ufloat      (Scene* scene, const char* uniform, float value);
void scene_umat4       (Scene* scene, const char* uniform, float value[16]);

#endif // _scene_h