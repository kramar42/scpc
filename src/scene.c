#include <stb_ds.h>
#include "main.h"
#include "scene.h"
#include "gl.h"

void scene_init(Scene* scene)
{
  scene->vertices = NULL;
  scene->indices  = NULL;

  // defaults
  if (!scene->polygon_mode)
    scene->polygon_mode = GL_LINE;

  if (!scene->primitive_type)
    scene->primitive_type = GL_TRIANGLES;

  if (!scene->focal_length)
    scene->focal_length = 1000.0f;

  // vertex layout
  glGenVertexArrays(1, &scene->vao);
  glBindVertexArray(scene->vao);

  // vertex data
  glGenBuffers(1, &scene->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, scene->vbo);

  // index data
  glGenBuffers(1, &scene->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene->ebo);

  // 3 floats per fertex, layout (location=0)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // shaders
  if (scene->vs_shader && scene->fs_shader)
  {
    scene->shader = gl_program(scene->vs_shader, scene->fs_shader);
  }
  else
  {
    scene->shader = 0;
  }

  // perspective
  scene_ufloat(scene, "focal",  scene->focal_length);
  scene_ufloat(scene, "aspect", self.window.aspect);
  scene_ufloat(scene, "width",  self.window.width  / 2.0f);
  scene_ufloat(scene, "height", self.window.height / 2.0f);
  scene_ufloat(scene, "depth",  self.window.depth  / 2.0f);
  scene_ufloat(scene, "front",  self.window.front);

  glBindVertexArray(0);
}

void scene_clear(Scene* scene)
{
  arrfree(scene->vertices);
  arrfree(scene->indices);
}

void scene_render(Scene* scene)
{
  glPolygonMode(GL_FRONT_AND_BACK, scene->polygon_mode);
  glUseProgram(scene->shader);
  glBindVertexArray(scene->vao);
  // TODO: why this needs rebinding after vao?
  glBindBuffer(GL_ARRAY_BUFFER,         scene->vbo);
  glBufferData(GL_ARRAY_BUFFER,         arrlenu(scene->vertices)*sizeof(scene->vertices[0]), scene->vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlenu(scene->indices)*sizeof(scene->indices[0]),   scene->indices,  GL_STATIC_DRAW);
  glDrawElements(scene->primitive_type, (int)arrlenu(scene->indices), GL_UNSIGNED_INT, 0);
}

void scene_ufloat(Scene* scene, const char* uniform, float value)
{
  uint32_t uniform_loc = glGetUniformLocation(scene->shader, uniform);
  glUseProgram(scene->shader);
  glUniform1f(uniform_loc, value);
}

void scene_umat4(Scene* scene, const char* uniform, float value[16])
{
  uint32_t uniform_loc = glGetUniformLocation(scene->shader, uniform);
  glUseProgram(scene->shader);
  glUniform1fv(uniform_loc, 16, value);
}
