#ifndef _main_h_
#define _main_h_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  const char* title;
  uint32_t    width;
  uint32_t    height;
  uint32_t    depth;
  float       aspect;
  bool        fullscreen;
  bool        vsync;
} Window;

typedef struct {
  bool        fps;
  float       last_frame;
  float       avg_fps;
  float       last_tick;
  float       tick;
} Client;

typedef struct {
  float       speed;
  float       x;
  float       y;
  float       z;
} Camera;

typedef struct {
  uint32_t    x;
  uint32_t    y;
  float       scroll;
  float       scale;
} Cursor;

typedef struct {
  Window      window;
  Client      client;
  Camera      camera;
  Cursor      cursor;
} Mind;

Mind self;

#endif // _main_h_