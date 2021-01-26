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
  float*  data;
} Channel;

Channel* new_chan   (size_t capacity);
size_t   chan_size  (Channel* chan);
void     put_chan   (Channel* chan, float dataptr);
float    reduce_chan(Channel* chan, float(*reducer)(float,float));

#ifdef _ps_impl_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ps.h"

Channel* new_chan(size_t capacity)
{
  Channel* c  = malloc(sizeof(Channel));
  c->capacity = capacity;
  c->current  = 0;
  c->full     = false;
  c->data     = (float*)malloc(capacity * sizeof(float));
  memset(c->data, 0, capacity * sizeof(float));
  // printf("chan: %p, offset: %p, end: %p\n", c, c->data, c->data + capacity);
  return c;
}

void put_chan(Channel* chan, float dataptr)
{
  chan->data[chan->current++] = dataptr;
  if (chan->current == chan->capacity)
  {
    chan->full    = true;
    chan->current = 0;
  }
}

size_t chan_size(Channel* chan)
{
  return chan->full ? chan->capacity : chan->current;
}

float reduce_chan(Channel* chan, float(*reducer)(float,float))
{
  float result = 0;
  for (size_t i = 0; i < chan_size(chan); i++)
    result = reducer(result, chan->data[i]);
  return result;
}

#endif // _ps_impl_
#endif // _ps_h_