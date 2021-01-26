#include <stb_ds.h>
#include "main.h"
#include "scene.h"
#include "gl.h"

void scene_init(Scene* scene)
{
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // shaders
  if (scene->vs_shader && scene->fs_shader)
  {
    scene->shader = gl_program(scene->vs_shader, scene->fs_shader);
    glUseProgram(scene->shader);
  }
  else
  {
    scene->shader = 0;
  }

  // wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  scene->vertices = NULL;
  scene->indices  = NULL;
}

void scene_clear(Scene* scene)
{
  arrfree(scene->vertices);
  arrfree(scene->indices);
}

void scene_render(Scene* scene)
{
  glBindVertexArray(scene->vao);
  glBufferData(GL_ARRAY_BUFFER, arrlenu(scene->vertices) * sizeof(float), scene->vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlenu(scene->indices) * sizeof(uint32_t), scene->indices, GL_STATIC_DRAW);
  glUseProgram(scene->shader);
  glDrawElements(GL_TRIANGLES, (int)arrlenu(scene->indices), GL_UNSIGNED_INT, 0);
}

void scene_ufloat(Scene* scene, const char* uniform, float value)
{
  glUseProgram(scene->shader);
  uint32_t uniform_loc = glGetUniformLocation(scene->shader, uniform);
  glUniform1f(uniform_loc, value);
}

void scene_umat4(Scene* scene, const char* uniform, float value[16])
{
  glUseProgram(scene->shader);
  uint32_t uniform_loc = glGetUniformLocation(scene->shader, uniform);
  glUniform1fv(uniform_loc, 16, value);
}
