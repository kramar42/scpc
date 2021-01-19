#ifndef _ps_h_
#define _ps_h_

#include <stdbool.h>

typedef struct {
  struct Cons* rest;
} Cons;

typedef struct {
  size_t  capacity;
  size_t  current;
  bool    full;
  double* data;
} Channel;

Channel* new_chan   (size_t capacity);
size_t   chan_size  (Channel* chan);
void     put_chan   (Channel* chan, double dataptr);
double   reduce_chan(Channel* chan, double(*reducer)(double,double));

#endif // _ps_h_