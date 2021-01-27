#ifndef _gl_h_
#define _gl_h_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "scene.h"
#include "ps.h"

typedef struct {
  GLFWmonitor* monitor;
  GLFWwindow*  window;
  Scene**      scenes;
  Channel*     fps;
} GL;

void        gl_init          (GL* gl);
void        gl_add_scene     (GL* gl, Scene* scene);
bool        gl_running       (GL* gl);
void        gl_clear         (GL* gl);
void        gl_update        (GL* gl);
void        gl_update_camera (GL* gl);

uint32_t    gl_program       (const char* vertex_filename, const char* fragment_filename);
void        gl_set_ufloat    (uint32_t program, const char* uniform, float value);
void        gl_set_u2float   (uint32_t program, const char* uniform, float first, float second);

#endif // _gl_h_