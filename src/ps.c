#include <stdio.h>
#include <stdlib.h>

#include "ps.h"

Channel* new_chan(size_t capacity)
{
  Channel* c  = malloc(sizeof(Channel));
  c->capacity = capacity;
  c->current  = 0;
  c->full     = false;
  c->data     = (double*)malloc(capacity * sizeof(double));
  memset(c->data, 0, capacity * sizeof(double));
  printf("chan: %p, offset: %p, end: %p\n", c, c->data, c->data + capacity);
  return c;
}

void put_chan(Channel* chan, double dataptr)
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

double reduce_chan(Channel* chan, double(*reducer)(double,double))
{
  double result = 0;
  for (size_t i = 0; i < chan_size(chan); i++)
    result = reducer(result, chan->data[i]);
  return result;
}
