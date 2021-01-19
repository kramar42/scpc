#ifndef _ga2_h_
#define _ga2_h_

typedef float  GA2[8];
typedef float* GA2p;

static const GA2 ga2_e0        = { 0, 1, 0, 0, 0, 0, 0, 0 };
static const GA2 ga2_e1        = { 0, 0, 1, 0, 0, 0, 0, 0 };
static const GA2 ga2_e2        = { 0, 0, 0, 1, 0, 0, 0, 0 };
static const GA2 ga2_e01       = { 0, 0, 0, 0, 1, 0, 0, 0 };
static const GA2 ga2_e02       = { 0, 0, 0, 0, 0, 1, 0, 0 };
static const GA2 ga2_e12       = { 0, 0, 0, 0, 0, 0, 1, 0 };
static const GA2 ga2_e012      = { 0, 0, 0, 0, 0, 0, 0, 1 };

static const char* ga2_basis[] = { "1","e0","e1","e2","e01","e02","e12","e012" };

void  ga2_print         (const GA2 a);

GA2p  ga2_point         (GA2 p, const float x, const float y);
GA2p  ga2_line          (GA2 l, const float x, const float y, const float z);
GA2p  ga2_rotor         (GA2 r, const GA2 point, const float angle);
GA2p  ga2_translator    (GA2 t, const GA2 point, const float dist);
GA2p  ga2_transform     (GA2 r, const GA2 trans, const GA2 elem);

float ga2_norm          (const GA2 x);
float ga2_inorm         (const GA2 x);

float ga2_distance      (const GA2 a, const GA2 b);

GA2p  ga2_normalized    (GA2 r, const GA2 a);
GA2p  ga2_reverse       (GA2 r, const GA2 a);
GA2p  ga2_dual          (GA2 r, const GA2 a);
GA2p  ga2_conjugate     (GA2 r, const GA2 a);
GA2p  ga2_involute      (GA2 r, const GA2 a);

GA2p  ga2_add           (GA2 r, const GA2 a, const GA2 b);
GA2p  ga2_add3          (GA2 r, const GA2 a, const GA2 b, const GA2 c);
GA2p  ga2_add_va        (GA2 r,              const size_t size, ...);
GA2p  ga2_adds          (GA2 r, const GA2 a, const float b);

GA2p  ga2_sub           (GA2 r, const GA2 a, const GA2 b);
GA2p  ga2_subs          (GA2 r, const GA2 a, const float b);

GA2p  ga2_mul           (GA2 r, const GA2 a, const GA2 b);
GA2p  ga2_muls          (GA2 r, const GA2 a, const float b);

GA2p  ga2_wedge         (GA2 r, const GA2 a, const GA2 b);
GA2p  ga2_regressive    (GA2 r, const GA2 a, const GA2 b);
GA2p  ga2_inner         (GA2 r, const GA2 a, const GA2 b);

#endif // _ga2_h