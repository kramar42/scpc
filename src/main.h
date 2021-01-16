#ifndef _main_h_
#define _main_h_

typedef struct {
  int width;
  int height;
} Client;

typedef struct {
  Client client;
} Mind;

Mind self = {0};

#endif