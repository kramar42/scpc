#include <stb_ds.h>
#include "main.h"
#include "scene.h"
#include "gl.h"

void scene_init(Scene* scene)
{
  // vertex data
  glGenBuffers(1, &scene->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, scene->vbo);

  // vertex layout
  glGenVertexArrays(1, &scene->vao);
  glBindVertexArray(scene->vao);

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
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  scene->vertices = NULL;
}

void scene_send_vertices(Scene* scene, float* vertices)
{
  scene->vertices = vertices;
  glBindBuffer(GL_ARRAY_BUFFER, scene->vbo);
  glBufferData(GL_ARRAY_BUFFER, arrlenu(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);
}

void scene_render(Scene* scene)
{
  // we output 3 points (3 coords each)
  glDrawArrays(GL_TRIANGLES, 0, (int)arrlenu(scene->vertices));
}

void scene_ufloat(Scene* scene, const char* uniform, float value)
{
  glUseProgram(scene->shader);
  uint32_t uniform_loc = glGetUniformLocation(scene->shader, uniform);
  glUniform1f(uniform_loc, value);
}

void scene_u2float(Scene* scene, const char* uniform, float first, float second)
{
  glUseProgram(scene->shader);
  uint32_t uniform_loc = glGetUniformLocation(scene->shader, uniform);
  glUniform2f(uniform_loc, first, second);
}

void scene_umat4(Scene* scene, const char* uniform, float value[16])
{
  glUseProgram(scene->shader);
  uint32_t uniform_loc = glGetUniformLocation(scene->shader, uniform);
  glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, value);
}
