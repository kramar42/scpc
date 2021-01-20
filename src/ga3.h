// 3D Projective Geometric Algebra
// Made from C++ code written by generator written by enki

#ifndef _ga3_h_
#define _ga3_h_

typedef float  GA3[16];
typedef float* GA3p;

static const char* ga3_basis[] = { "1","e0","e1","e2","e3","e01","e02","e03", "e12",
                                  "e31","e23","e021","e013","e032","e123","e0123" };

static const GA3 ga3_e0        = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const GA3 ga3_e1        = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const GA3 ga3_e2        = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const GA3 ga3_e3        = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const GA3 ga3_e01       = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const GA3 ga3_e02       = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static const GA3 ga3_e03       = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
static const GA3 ga3_e12       = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
static const GA3 ga3_e31       = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };
static const GA3 ga3_e23       = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 };
static const GA3 ga3_e021      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 };
static const GA3 ga3_e013      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 };
static const GA3 ga3_e032      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 };
static const GA3 ga3_e123      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 };
static const GA3 ga3_e0123     = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };


void         ga3_print           (const GA3 a);

// constructors
GA3p         ga3_point           (GA3 p, float x, float y, float z);
GA3p         ga3_plane           (GA3 p, float a, float b, float c, float d);
GA3p         ga3_rotor           (GA3 r, const GA3 line, const float angle);
GA3p         ga3_translator      (GA3 t, GA3 line, float dist);


// norms
float        ga3_norm            (const GA3 x);
float        ga3_inorm           (const GA3 x);
float        ga3_distance        (const GA3 a, const GA3 b);

// unary
GA3p         ga3_normalized      (GA3 r, const GA3   x);
GA3p         ga3_reverse         (GA3 r, const GA3   a);
GA3p         ga3_dual            (GA3 r, const GA3   a);
GA3p         ga3_conjugate       (GA3 r, const GA3   a);
GA3p         ga3_involute        (GA3 r, const GA3   a);

// operators
GA3p         ga3_mul             (GA3 r, const GA3   a, const GA3   b);
GA3p         ga3_meet            (GA3 r, const GA3   a, const GA3   b);
GA3p         ga3_join            (GA3 r, const GA3   a, const GA3   b);
GA3p         ga3_dot             (GA3 r, const GA3   a, const GA3   b);
GA3p         ga3_add             (GA3 r, const GA3   a, const GA3   b);
GA3p         ga3_add4            (GA3 r, const GA3   a, const GA3   b, const GA3 c, const GA3 d);
GA3p         ga3_sub             (GA3 r, const GA3   a, const GA3   b);
GA3p         ga3_muls            (GA3 r, const GA3   a, const float b);
GA3p         ga3_smul            (GA3 r, const float a, const GA3   b);
GA3p         ga3_adds            (GA3 r, const GA3   a, const float b);
GA3p         ga3_sadd            (GA3 r, const float a, const GA3   b);
GA3p         ga3_subs            (GA3 r, const GA3   a, const float b);
GA3p         ga3_ssub            (GA3 r, const float a, const GA3   b);

#endif // _ga3_h_