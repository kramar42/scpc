#ifndef _main_h_
#define _main_h_

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
  int width;
  int height;
  float aspect;
  bool fullscreen;
  double last_frame;
  double avg_fps;
} Client;

typedef struct {
  int x;
  int y;
} Cursor;

typedef struct {
  Client client;
  Cursor cursor;
} Mind;

Mind self;

void success();
void error(const char* message);

#endif