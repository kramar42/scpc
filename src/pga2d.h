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

static PGA2Dp pga2d_new        ();
static void   pga2d_print      (const PGA2D a);

static float  pga2d_norm       (const PGA2D a);
static float  pga2d_inorm      (const PGA2D a);
static PGA2Dp pga2d_normalized (const PGA2D a);

static PGA2Dp pga2d_reverse    (const PGA2D a);
static PGA2Dp pga2d_dual       (const PGA2D a);
static PGA2Dp pga2d_conjugate  (const PGA2D a);
static PGA2Dp pga2d_involute   (const PGA2D a);

static PGA2Dp pga2d_add        (const PGA2D a, const PGA2D b);
static PGA2Dp pga2d_adds       (const PGA2D a, const float b);
static PGA2Dp pga2d_sub        (const PGA2D a, const PGA2D b);
static PGA2Dp pga2d_subs       (const PGA2D a, const float b);
static PGA2Dp pga2d_mul        (const PGA2D a, const PGA2D b);
static PGA2Dp pga2d_muls       (const PGA2D a, const float b);

static PGA2Dp pga2d_wedge      (const PGA2D a, const PGA2D b);
static PGA2Dp pga2d_regressive (const PGA2D a, const PGA2D b);
static PGA2Dp pga2d_inner      (const PGA2D a, const PGA2D b);

#endif