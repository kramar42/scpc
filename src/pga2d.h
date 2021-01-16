#ifndef _pga2d_h_
#define _pga2d_h_

typedef float  PGA2D[8];
typedef float* PGA2Dp;

static const PGA2D pga2d_e0      = { 0, 1, 0, 0, 0, 0, 0, 0 };
static const PGA2D pga2d_e1      = { 0, 0, 1, 0, 0, 0, 0, 0 };
static const PGA2D pga2d_e2      = { 0, 0, 0, 1, 0, 0, 0, 0 };
static const PGA2D pga2d_e01     = { 0, 0, 0, 0, 1, 0, 0, 0 };
static const PGA2D pga2d_e02     = { 0, 0, 0, 0, 0, 1, 0, 0 };
static const PGA2D pga2d_e12     = { 0, 0, 0, 0, 0, 0, 1, 0 };
static const PGA2D pga2d_e012    = { 0, 0, 0, 0, 0, 0, 0, 1 };

static const char* pga2d_basis[] = { "1","e0","e1","e2","e01","e02","e12","e012" };

float  pga2d_norm       (const PGA2D a);
float  pga2d_inorm      (const PGA2D a);

PGA2Dp pga2d_normalized (PGA2D r, const PGA2D a);
PGA2Dp pga2d_reverse    (PGA2D r, const PGA2D a);
PGA2Dp pga2d_dual       (PGA2D r, const PGA2D a);
PGA2Dp pga2d_conjugate  (PGA2D r, const PGA2D a);
PGA2Dp pga2d_involute   (PGA2D r, const PGA2D a);

PGA2Dp pga2d_add        (PGA2D r, const PGA2D a, const PGA2D b);
PGA2Dp pga2d_add3       (PGA2D r, const PGA2D a, const PGA2D b, const PGA2D c);
PGA2Dp pga2d_add_va     (PGA2D r, size_t size, ...);
PGA2Dp pga2d_adds       (PGA2D r, const PGA2D a, const float b);

PGA2Dp pga2d_sub        (PGA2D r, const PGA2D a, const PGA2D b);
PGA2Dp pga2d_subs       (PGA2D r, const PGA2D a, const float b);

PGA2Dp pga2d_mul        (PGA2D r, const PGA2D a, const PGA2D b);
PGA2Dp pga2d_muls       (PGA2D r, const PGA2D a, const float b);

PGA2Dp pga2d_wedge      (PGA2D r, const PGA2D a, const PGA2D b);
PGA2Dp pga2d_regressive (PGA2D r, const PGA2D a, const PGA2D b);
PGA2Dp pga2d_inner      (PGA2D r, const PGA2D a, const PGA2D b);

#endif