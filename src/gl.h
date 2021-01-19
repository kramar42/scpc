#ifndef _gl_h_
#define _gl_h_

#include "main.h"

GLFWwindow* gl_init        (void);
uint32_t    gl_program     (const char* vertex_filename, const char* fragment_filename);
void        gl_set_ufloat  (uint32_t program, const char* uniform, float value);
void        gl_set_u2float (uint32_t program, const char* uniform, float first, float second);

#endif // _gl_h_