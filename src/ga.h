#ifndef _ga_h_
#define _ga_h_

#include "pga2d.h"

void   ga_print      (const PGA2D a);

PGA2Dp ga_point      (PGA2D point,      const float x, const float y);
PGA2Dp ga_line       (PGA2D line,       const float a, const float b, const float c);
PGA2Dp ga_rotor      (PGA2D rotor,      const PGA2D point, const float angle);
PGA2Dp ga_translator (PGA2D translator, const PGA2D point, const float dist);
PGA2Dp ga_transform  (PGA2D result,     const PGA2D trans, const PGA2D elem);

#endif