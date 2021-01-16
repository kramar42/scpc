#ifndef _ga_h_
#define _ga_h_

#include "pga2d.h"

void   ga_print      (const PGA2D a);

PGA2Dp ga_point      (PGA2Dp point,      const float x, const float y);
PGA2Dp ga_line       (PGA2Dp line,       const float a, const float b, const float c);
PGA2Dp ga_rotor      (PGA2Dp rotor,      PGA2D point, float angle);
PGA2Dp ga_translator (PGA2Dp translator, PGA2D point, float dist);

#endif