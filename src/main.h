#ifndef _main_h_
#define _main_h_

typedef struct {
  int width;
  int height;
} Client;

typedef struct {
  int x;
  int y;
} Cursor;

typedef struct {
  Client client;
  Cursor cursor;
} Mind;

Mind self = {0};

#endif