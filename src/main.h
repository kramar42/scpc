#ifndef _main_h_
#define _main_h_

#include <stdbool.h>
#include <stdint.h>

#include "ga3.h"

typedef struct {
  const char* title;
  uint32_t    size;
  uint32_t    width;
  uint32_t    height;
  uint32_t    depth;
  float       front;
  float       aspect;
  bool        fullscreen;
  bool        vsync;
} Window;

typedef struct {
  bool        fps;
  float       last_fps;
  float       last_frame;
  float       avg_fps;
  float       tick;
  float       last_tick;
} Stats;

typedef struct {
  GA3p        position;
  GA3p        forward;
  GA3p        right;
  GA3p        up;
  float       speed;
  float       sensitivity;
} Camera;

typedef struct {
  float       last_x;
  float       last_y;
  float       scroll;
  float       scale;
} Cursor;

typedef struct {
  Window      window;
  Stats       stats;
  Camera      camera;
  Cursor      cursor;
} Mind;

Mind self;

#endif // _main_h_