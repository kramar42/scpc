#ifndef _main_h_
#define _main_h_

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
  int width;
  int height;
  float aspect;
  bool fullscreen;
  bool vsync;
  double last_frame;
  double avg_fps;
} Client;

typedef struct {
  int    x;
  int    y;
  double scroll;
  double scale;
} Cursor;

typedef struct {
  Client client;
  Cursor cursor;
} Mind;

Mind self;

#endif // _main_h_