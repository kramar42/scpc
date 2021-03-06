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


void         ga3_print           (const char* m, const GA3 a);

// constructors
GA3p         ga3_id              (GA3 r, const GA3 a);
GA3p         ga3_point           (GA3 p, const float x,   const float y,      const float z);
GA3p         ga3_plane           (GA3 p, const float x,   const float y,      const float z, const float w);
GA3p         ga3_rotor           (GA3 r, const GA3 line,  const float angle);
GA3p         ga3_translator      (GA3 t, const GA3 line,  const float dist);
GA3p         ga3_transform       (GA3 r, const GA3 trans, const GA3   elem);
GA3p         ga3_combine         (GA3 r, const GA3** trs, size_t n);
GA3p         ga3_circle          (GA3 r, const GA3 line,  const float radius, const float t);
GA3p         ga3_torus           (GA3 r, const float s,   const float t,      const float r1, const GA3 l1, const float r2, const GA3 l2);

// norms
float        ga3_norm            (const GA3 x);
float        ga3_inorm           (const GA3 x);
float        ga3_distance        (const GA3 a, const GA3 b);
float        ga3_angle           (const GA3 a, const GA3 b);

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

GA3p         ga3_point_on_torus  (GA3 r, const float s, const float t);

#ifdef _ga3_impl_

#include <stdio.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

void ga3_print(const char* msg, const GA3 a)
{
  printf("%s", msg);
  size_t n = 0;
  for (size_t i = 0, j = 0; i < 16; i++)
  {
    if (a[i] != 0.0f)
    {
      n++;
      printf("%s%0.7g%s",
        // sign between terms
        (j > 0) ? " + " : "",
        // coeff
        a[i],
        (i == 0) ? "" : ga3_basis[i]);
      j++;
    };
  }
  if (n == 0) printf("0");
  printf("\n");
}

GA3p ga3_id(GA3 r, const GA3 a)
{
  r[ 0] = a[ 0];
  r[ 1] = a[ 1];
  r[ 2] = a[ 2];
  r[ 3] = a[ 3];
  r[ 4] = a[ 4];
  r[ 5] = a[ 5];
  r[ 6] = a[ 6];
  r[ 7] = a[ 7];
  r[ 8] = a[ 8];
  r[ 9] = a[ 9];
  r[10] = a[10];
  r[11] = a[11];
  r[12] = a[12];
  r[13] = a[13];
  r[14] = a[14];
  r[15] = a[15];
  return r;

}

// PGA points are trivectors.
// A point is just a homogeneous point, euclidean coordinates plus the origin
GA3p ga3_point(GA3 p, const float x, const float y, const float z)
{
  p[ 0] = 0;
  p[ 1] = 0;
  p[ 2] = 0;
  p[ 3] = 0;
  p[ 4] = 0;
  p[ 5] = 0;
  p[ 6] = 0;
  p[ 7] = 0;
  p[ 8] = 0;
  p[ 9] = 0;
  p[10] = 0;
  p[11] = z;
  p[12] = y;
  p[13] = x;
  p[14] = 1;
  p[15] = 0;
  return p;
}

// PGA is plane based. Vectors are planes. (think linear functionals)
// A plane is defined using its homogenous equation ax + by + cz + d = 0
GA3p ga3_plane(GA3 p, const float a, const float b, const float c, const float d)
{
  p[ 0] = 0;
  p[ 1] = d;
  p[ 2] = a;
  p[ 3] = b;
  p[ 4] = c;
  p[ 5] = 0;
  p[ 6] = 0;
  p[ 7] = 0;
  p[ 8] = 0;
  p[ 9] = 0;
  p[10] = 0;
  p[11] = 0;
  p[12] = 0;
  p[13] = 0;
  p[14] = 0;
  p[15] = 0;
  return p;
}

// A rotor (Euclidean line)
GA3p ga3_rotor(GA3 r, const GA3 line, const float angle)
{
  GA3 a = {0}, b = {0};
  return ga3_sadd(r,
    cosf(angle),
    ga3_smul(a,
      sinf(angle),
      ga3_normalized(b,
        line)));
}

// A translator (Ideal line)
GA3p ga3_translator(GA3 t, const GA3 line, const float dist)
{
  GA3 a = {0};
  return ga3_sadd(t,
    1.0f,
    ga3_smul(a,
      dist / 2.0f,
      line));
}

GA3p ga3_transform(GA3 r, const GA3 trans, const GA3 elem)
{
  GA3 e, a, b;
  ga3_id(e, elem);
  return ga3_mul(r,
    ga3_mul(a,
      trans,
      e),
    ga3_reverse(b,
      trans));
}

GA3p ga3_combine(GA3 r, const GA3** trs, size_t n)
{
  assert(n > 0);
  GA3 tmp[2];
  ga3_sadd(tmp[0], 0, *trs[0]);
  for (size_t i = 1; i < n; i++)
    ga3_mul(tmp[i%2], *trs[i], tmp[(i-1)%2]); //, *trs[i]);
  return ga3_sadd(r, 0, tmp[(n-1)%2]);
}

// circle(t) with t going from 0 to 1.
GA3p ga3_circle(GA3 r, const GA3 line, const float radius, const float t)
{
  GA3 a = {0}, b = {0}, c = {0};
  return ga3_mul(r,
    ga3_rotor(a,
      line,
      (float) (t * 2.0f * M_PI)),
    ga3_translator(b,
      ga3_mul(c,
        ga3_e1,
        ga3_e0),
      radius));
}

// a torus is now the product of two circles.
GA3p ga_torus(GA3 r, const float s, const float t, const float r1, const GA3 l1, const float r2, const GA3 l2)
{
  GA3 a, b;
  return ga3_mul(r,
    ga3_circle(a,
      l2, r2, s),
    ga3_circle(b,
      l1, r1, t));
}

// and to sample its points we simply sandwich the origin ..
GA3p ga_point_on_torus(GA3 r, const float s, const float t)
{
  GA3 to, l1, l2;
  ga_torus(to, s, t, 0.25f,
    ga3_mul(l1,
      ga3_e1,
      ga3_e2),
    0.6f,
    ga3_mul(l2,
      ga3_e1, ga3_e3));
  return ga3_transform(r, to, ga3_e123);
}

// Norm
float ga3_norm(const GA3 x)
{
  GA3 a, b;
  return sqrtf(fabsf(
    ga3_mul(a,
      ga3_conjugate(b,
        x),
      x)
    [0]));
}

// Inverse norm
float ga3_inorm(const GA3 x)
{
  GA3 a = {0};
  return ga3_norm(
    ga3_dual(a,
      x));
}

// ???
float ga3_distance(const GA3 a, const GA3 b)
{
  GA3 d = {0};
  return ga3_norm(
    ga3_join(d,
      a, b));
}

float ga3_angle(const GA3 a, const GA3 b)
{
  GA3 na, nb, r;
  ga3_dot(r,
    ga3_normalized(na, a),
    ga3_normalized(nb, b));
  return acosf(r[0]);
}

// Normalize
GA3p ga3_normalized(GA3 r, const GA3 x)
{
  float n = ga3_norm(x);
  if (n == 0) return ga3_id(r, x);

  return ga3_smul(r,
    1/n, x);
}

// Reverse the order of the basis blades (~)
GA3p ga3_reverse(GA3 r, const GA3 a)
{
  r[ 0] =  a[ 0];
  r[ 1] =  a[ 1];
  r[ 2] =  a[ 2];
  r[ 3] =  a[ 3];
  r[ 4] =  a[ 4];
  r[ 5] = -a[ 5];
  r[ 6] = -a[ 6];
  r[ 7] = -a[ 7];
  r[ 8] = -a[ 8];
  r[ 9] = -a[ 9];
  r[10] = -a[10];
  r[11] = -a[11];
  r[12] = -a[12];
  r[13] = -a[13];
  r[14] = -a[14];
  r[15] =  a[15];
  return r;
};

// Poincare duality operator (!)
GA3p ga3_dual(GA3 r, const GA3 a)
{
  r[ 0] = a[15];
  r[ 1] = a[14];
  r[ 2] = a[13];
  r[ 3] = a[12];
  r[ 4] = a[11];
  r[ 5] = a[10];
  r[ 6] = a[ 9];
  r[ 7] = a[ 8];
  r[ 8] = a[ 7];
  r[ 9] = a[ 6];
  r[10] = a[ 5];
  r[11] = a[ 4];
  r[12] = a[ 3];
  r[13] = a[ 2];
  r[14] = a[ 1];
  r[15] = a[ 0];
  return r;
};

// Clifford Conjugation
GA3p ga3_conjugate(GA3 r, const GA3 a)
{
  r[ 0] =  a[ 0];
  r[ 1] = -a[ 1];
  r[ 2] = -a[ 2];
  r[ 3] = -a[ 3];
  r[ 4] = -a[ 4];
  r[ 5] = -a[ 5];
  r[ 6] = -a[ 6];
  r[ 7] = -a[ 7];
  r[ 8] = -a[ 8];
  r[ 9] = -a[ 9];
  r[10] = -a[10];
  r[11] =  a[11];
  r[12] =  a[12];
  r[13] =  a[13];
  r[14] =  a[14];
  r[15] =  a[15];
  return r;
};

// Main involution
GA3p ga3_involute(GA3 r, const GA3 a)
{
  r[ 0] =  a[ 0];
  r[ 1] = -a[ 1];
  r[ 2] = -a[ 2];
  r[ 3] = -a[ 3];
  r[ 4] = -a[ 4];
  r[ 5] =  a[ 5];
  r[ 6] =  a[ 6];
  r[ 7] =  a[ 7];
  r[ 8] =  a[ 8];
  r[ 9] =  a[ 9];
  r[10] =  a[10];
  r[11] = -a[11];
  r[12] = -a[12];
  r[13] = -a[13];
  r[14] = -a[14];
  r[15] =  a[15];
  return r;
};

// The geometric product (*)
GA3p ga3_mul(GA3 r, const GA3 a, const GA3 b)
{
  r[ 0] = b[ 0]*a[0] + b[ 2]*a[2] + b[ 3]*a[3] + b[ 4]*a[4] - b[ 8]*a[8] - b[ 9]*a[9] - b[10]*a[10] - b[14]*a[14];
  r[ 1] = b[ 1]*a[0] + b[ 0]*a[1] - b[ 5]*a[2] - b[ 6]*a[3] - b[ 7]*a[4] + b[ 2]*a[5] + b[ 3]*a[ 6] + b[ 4]*a[ 7] + b[11]*a[8] + b[12]*a[9] + b[13]*a[10] + b[ 8]*a[11] + b[ 9]*a[12] + b[10]*a[13] + b[15]*a[14] - b[14]*a[15];
  r[ 2] = b[ 2]*a[0] + b[ 0]*a[2] - b[ 8]*a[3] + b[ 9]*a[4] + b[ 3]*a[8] - b[ 4]*a[9] - b[14]*a[10] - b[10]*a[14];
  r[ 3] = b[ 3]*a[0] + b[ 8]*a[2] + b[ 0]*a[3] - b[10]*a[4] - b[ 2]*a[8] - b[14]*a[9] + b[ 4]*a[10] - b[ 9]*a[14];
  r[ 4] = b[ 4]*a[0] - b[ 9]*a[2] + b[10]*a[3] + b[ 0]*a[4] - b[14]*a[8] + b[ 2]*a[9] - b[ 3]*a[10] - b[ 8]*a[14];
  r[ 5] = b[ 5]*a[0] + b[ 2]*a[1] - b[ 1]*a[2] - b[11]*a[3] + b[12]*a[4] + b[ 0]*a[5] - b[ 8]*a[ 6] + b[ 9]*a[ 7] + b[ 6]*a[8] - b[ 7]*a[9] - b[15]*a[10] - b[ 3]*a[11] + b[ 4]*a[12] + b[14]*a[13] - b[13]*a[14] - b[10]*a[15];
  r[ 6] = b[ 6]*a[0] + b[ 3]*a[1] + b[11]*a[2] - b[ 1]*a[3] - b[13]*a[4] + b[ 8]*a[5] + b[ 0]*a[ 6] - b[10]*a[ 7] - b[ 5]*a[8] - b[15]*a[9] + b[ 7]*a[10] + b[ 2]*a[11] + b[14]*a[12] - b[ 4]*a[13] - b[12]*a[14] - b[ 9]*a[15];
  r[ 7] = b[ 7]*a[0] + b[ 4]*a[1] - b[12]*a[2] + b[13]*a[3] - b[ 1]*a[4] - b[ 9]*a[5] + b[10]*a[ 6] + b[ 0]*a[ 7] - b[15]*a[8] + b[ 5]*a[9] - b[ 6]*a[10] + b[14]*a[11] - b[ 2]*a[12] + b[ 3]*a[13] - b[11]*a[14] - b[ 8]*a[15];
  r[ 8] = b[ 8]*a[0] + b[ 3]*a[2] - b[ 2]*a[3] + b[14]*a[4] + b[ 0]*a[8] + b[10]*a[9] - b[ 9]*a[10] + b[ 4]*a[14];
  r[ 9] = b[ 9]*a[0] - b[ 4]*a[2] + b[14]*a[3] + b[ 2]*a[4] - b[10]*a[8] + b[ 0]*a[9] + b[ 8]*a[10] + b[ 3]*a[14];
  r[10] = b[10]*a[0] + b[14]*a[2] + b[ 4]*a[3] - b[ 3]*a[4] + b[ 9]*a[8] - b[ 8]*a[9] + b[ 0]*a[10] + b[ 2]*a[14];
  r[11] = b[11]*a[0] - b[ 8]*a[1] + b[ 6]*a[2] - b[ 5]*a[3] + b[15]*a[4] - b[ 3]*a[5] + b[ 2]*a[ 6] - b[14]*a[ 7] - b[ 1]*a[8] + b[13]*a[9] - b[12]*a[10] + b[ 0]*a[11] + b[10]*a[12] - b[ 9]*a[13] + b[ 7]*a[14] - b[ 4]*a[15];
  r[12] = b[12]*a[0] - b[ 9]*a[1] - b[ 7]*a[2] + b[15]*a[3] + b[ 5]*a[4] + b[ 4]*a[5] - b[14]*a[ 6] - b[ 2]*a[ 7] - b[13]*a[8] - b[ 1]*a[9] + b[11]*a[10] - b[10]*a[11] + b[ 0]*a[12] + b[ 8]*a[13] + b[ 6]*a[14] - b[ 3]*a[15];
  r[13] = b[13]*a[0] - b[10]*a[1] + b[15]*a[2] + b[ 7]*a[3] - b[ 6]*a[4] - b[14]*a[5] - b[ 4]*a[ 6] + b[ 3]*a[ 7] + b[12]*a[8] - b[11]*a[9] - b[ 1]*a[10] + b[ 9]*a[11] - b[ 8]*a[12] + b[ 0]*a[13] + b[ 5]*a[14] - b[ 2]*a[15];
  r[14] = b[14]*a[0] + b[10]*a[2] + b[ 9]*a[3] + b[ 8]*a[4] + b[ 4]*a[8] + b[ 3]*a[9] + b[ 2]*a[10] + b[ 0]*a[14];
  r[15] = b[15]*a[0] + b[14]*a[1] + b[13]*a[2] + b[12]*a[3] + b[11]*a[4] + b[10]*a[5] + b[ 9]*a[ 6] + b[ 8]*a[ 7] + b[ 7]*a[8] + b[ 6]*a[9] + b[ 5]*a[10] - b[ 4]*a[11] - b[ 3]*a[12] - b[ 2]*a[13] - b[ 1]*a[14] + b[ 0]*a[15];
  return r;
};

// The outer product (^)
GA3p ga3_meet(GA3 r, const GA3 a, const GA3 b)
{
  r[ 0] = b[ 0]*a[0];
  r[ 1] = b[ 1]*a[0] + b[ 0]*a[1];
  r[ 2] = b[ 2]*a[0] + b[ 0]*a[2];
  r[ 3] = b[ 3]*a[0] + b[ 0]*a[3];
  r[ 4] = b[ 4]*a[0] + b[ 0]*a[4];
  r[ 5] = b[ 5]*a[0] + b[ 2]*a[1] - b[ 1]*a[2] + b[ 0]*a[ 5];
  r[ 6] = b[ 6]*a[0] + b[ 3]*a[1] - b[ 1]*a[3] + b[ 0]*a[ 6];
  r[ 7] = b[ 7]*a[0] + b[ 4]*a[1] - b[ 1]*a[4] + b[ 0]*a[ 7];
  r[ 8] = b[ 8]*a[0] + b[ 3]*a[2] - b[ 2]*a[3] + b[ 0]*a[ 8];
  r[ 9] = b[ 9]*a[0] - b[ 4]*a[2] + b[ 2]*a[4] + b[ 0]*a[ 9];
  r[10] = b[10]*a[0] + b[ 4]*a[3] - b[ 3]*a[4] + b[ 0]*a[10];
  r[11] = b[11]*a[0] - b[ 8]*a[1] + b[ 6]*a[2] - b[ 5]*a[ 3] - b[ 3]*a[5] + b[ 2]*a[6] - b[1]*a[ 8] + b[0]*a[11];
  r[12] = b[12]*a[0] - b[ 9]*a[1] - b[ 7]*a[2] + b[ 5]*a[ 4] + b[ 4]*a[5] - b[ 2]*a[7] - b[1]*a[ 9] + b[0]*a[12];
  r[13] = b[13]*a[0] - b[10]*a[1] + b[ 7]*a[3] - b[ 6]*a[ 4] - b[ 4]*a[6] + b[ 3]*a[7] - b[1]*a[10] + b[0]*a[13];
  r[14] = b[14]*a[0] + b[10]*a[2] + b[ 9]*a[3] + b[ 8]*a[ 4] + b[ 4]*a[8] + b[ 3]*a[9] + b[2]*a[10] + b[0]*a[14];
  r[15] = b[15]*a[0] + b[14]*a[1] + b[13]*a[2] + b[12]*a[ 3] + b[11]*a[4] + b[10]*a[5] + b[9]*a[ 6] + b[8]*a[ 7] + b[7]*a[8] + b[6]*a[9] + b[5]*a[10] - b[4]*a[11] - b[3]*a[12] - b[2]*a[13] - b[1]*a[14] + b[0]*a[15];
  return r;
};

// The regressive product (&)
GA3p ga3_join(GA3 r, const GA3 a, const GA3 b)
{
  r[15] = a[15]*b[15];
  r[14] = a[14]*b[15] - a[15]*b[14];
  r[13] = a[13]*b[15] - a[15]*b[13];
  r[12] = a[12]*b[15] - a[15]*b[12];
  r[11] = a[11]*b[15] - a[15]*b[11];
  r[10] = a[10]*b[15] + a[13]*b[14] - a[14]*b[13] + a[15]*b[10];
  r[ 9] = a[ 9]*b[15] + a[12]*b[14] - a[14]*b[12] + a[15]*b[ 9];
  r[ 8] = a[ 8]*b[15] + a[11]*b[14] - a[14]*b[11] + a[15]*b[ 8];
  r[ 7] = a[ 7]*b[15] + a[12]*b[13] - a[13]*b[12] + a[15]*b[ 7];
  r[ 6] = a[ 6]*b[15] - a[11]*b[13] + a[13]*b[11] + a[15]*b[ 6];
  r[ 5] = a[ 5]*b[15] + a[11]*b[12] - a[12]*b[11] + a[15]*b[ 5];
  r[ 4] = a[ 4]*b[15] + a[ 7]*b[14] - a[ 9]*b[13] + a[10]*b[12] + a[12]*b[10] - a[13]*b[ 9] + a[14]*b[7] + a[15]*b[4];
  r[ 3] = a[ 3]*b[15] + a[ 6]*b[14] + a[ 8]*b[13] - a[10]*b[11] - a[11]*b[10] + a[13]*b[ 8] + a[14]*b[6] + a[15]*b[3];
  r[ 2] = a[ 2]*b[15] + a[ 5]*b[14] - a[ 8]*b[12] + a[ 9]*b[11] + a[11]*b[ 9] - a[12]*b[ 8] + a[14]*b[5] + a[15]*b[2];
  r[ 1] = a[ 1]*b[15] - a[ 5]*b[13] - a[ 6]*b[12] - a[ 7]*b[11] - a[11]*b[ 7] - a[12]*b[ 6] - a[13]*b[5] + a[15]*b[1];
  r[ 0] = a[ 0]*b[15] - a[ 1]*b[14] - a[ 2]*b[13] - a[ 3]*b[12] - a[ 4]*b[11] + a[ 5]*b[10] + a[ 6]*b[9] + a[ 7]*b[8] + a[8]*b[7] + a[9]*b[6] + a[10]*b[5] + a[11]*b[4] + a[12]*b[3] + a[13]*b[2] + a[14]*b[1] + a[15]*b[0];
  return r;
};

// The inner product (|)
GA3p ga3_dot(GA3 r, const GA3 a, const GA3 b)
{
  r[ 0] = b[ 0]*a[0] + b[ 2]*a[ 2] + b[ 3]*a[ 3] + b[ 4]*a[ 4] - b[ 8]*a[ 8] - b[9 ]*a[ 9] - b[10]*a[10] - b[14]*a[14];
  r[ 1] = b[ 1]*a[0] + b[ 0]*a[ 1] - b[ 5]*a[ 2] - b[ 6]*a[ 3] - b[ 7]*a[ 4] + b[2 ]*a[ 5] + b[ 3]*a[ 6] + b[ 4]*a[ 7] + b[11]*a[8] + b[12]*a[9] + b[13]*a[10] + b[8]*a[11] + b[9]*a[12] + b[10]*a[13] + b[15]*a[14] - b[14]*a[15];
  r[ 2] = b[ 2]*a[0] + b[ 0]*a[ 2] - b[ 8]*a[ 3] + b[ 9]*a[ 4] + b[ 3]*a[ 8] - b[4 ]*a[ 9] - b[14]*a[10] - b[10]*a[14];
  r[ 3] = b[ 3]*a[0] + b[ 8]*a[ 2] + b[ 0]*a[ 3] - b[10]*a[ 4] - b[ 2]*a[ 8] - b[14]*a[ 9] + b[ 4]*a[10] - b[ 9]*a[14];
  r[ 4] = b[ 4]*a[0] - b[ 9]*a[ 2] + b[10]*a[ 3] + b[ 0]*a[ 4] - b[14]*a[ 8] + b[2 ]*a[ 9] - b[ 3]*a[10] - b[ 8]*a[14];
  r[ 5] = b[ 5]*a[0] - b[11]*a[ 3] + b[12]*a[ 4] + b[ 0]*a[ 5] - b[15]*a[10] - b[3 ]*a[11] + b[ 4]*a[12] - b[10]*a[15];
  r[ 6] = b[ 6]*a[0] + b[11]*a[ 2] - b[13]*a[ 4] + b[ 0]*a[ 6] - b[15]*a[ 9] + b[2 ]*a[11] - b[ 4]*a[13] - b[ 9]*a[15];
  r[ 7] = b[ 7]*a[0] - b[12]*a[ 2] + b[13]*a[ 3] + b[ 0]*a[ 7] - b[15]*a[ 8] - b[2 ]*a[12] + b[ 3]*a[13] - b[ 8]*a[15];
  r[ 8] = b[ 8]*a[0] + b[14]*a[ 4] + b[ 0]*a[ 8] + b[ 4]*a[14];
  r[ 9] = b[ 9]*a[0] + b[14]*a[ 3] + b[ 0]*a[ 9] + b[ 3]*a[14];
  r[10] = b[10]*a[0] + b[14]*a[ 2] + b[ 0]*a[10] + b[ 2]*a[14];
  r[11] = b[11]*a[0] + b[15]*a[ 4] + b[ 0]*a[11] - b[ 4]*a[15];
  r[12] = b[12]*a[0] + b[15]*a[ 3] + b[ 0]*a[12] - b[ 3]*a[15];
  r[13] = b[13]*a[0] + b[15]*a[ 2] + b[ 0]*a[13] - b[ 2]*a[15];
  r[14] = b[14]*a[0] + b[ 0]*a[14];
  r[15] = b[15]*a[0] + b[ 0]*a[15];
  return r;
};

// Multivector addition (+)
GA3p ga3_add(GA3 r, const GA3 a, const GA3 b)
{
  r[ 0] = a[ 0] + b[ 0];
  r[ 1] = a[ 1] + b[ 1];
  r[ 2] = a[ 2] + b[ 2];
  r[ 3] = a[ 3] + b[ 3];
  r[ 4] = a[ 4] + b[ 4];
  r[ 5] = a[ 5] + b[ 5];
  r[ 6] = a[ 6] + b[ 6];
  r[ 7] = a[ 7] + b[ 7];
  r[ 8] = a[ 8] + b[ 8];
  r[ 9] = a[ 9] + b[ 9];
  r[10] = a[10] + b[10];
  r[11] = a[11] + b[11];
  r[12] = a[12] + b[12];
  r[13] = a[13] + b[13];
  r[14] = a[14] + b[14];
  r[15] = a[15] + b[15];
  return r;
};

GA3p ga3_add4(GA3 r, const GA3 a, const GA3 b, const GA3 c, const GA3 d)
{
  r[ 0] = a[ 0] + b[ 0] + c[ 0] + d[ 0];
  r[ 1] = a[ 1] + b[ 1] + c[ 1] + d[ 1];
  r[ 2] = a[ 2] + b[ 2] + c[ 2] + d[ 2];
  r[ 3] = a[ 3] + b[ 3] + c[ 3] + d[ 3];
  r[ 4] = a[ 4] + b[ 4] + c[ 4] + d[ 4];
  r[ 5] = a[ 5] + b[ 5] + c[ 5] + d[ 5];
  r[ 6] = a[ 6] + b[ 6] + c[ 6] + d[ 6];
  r[ 7] = a[ 7] + b[ 7] + c[ 7] + d[ 7];
  r[ 8] = a[ 8] + b[ 8] + c[ 8] + d[ 8];
  r[ 9] = a[ 9] + b[ 9] + c[ 9] + d[ 9];
  r[10] = a[10] + b[10] + c[10] + d[10];
  r[11] = a[11] + b[11] + c[11] + d[11];
  r[12] = a[12] + b[12] + c[12] + d[12];
  r[13] = a[13] + b[13] + c[13] + d[13];
  r[14] = a[14] + b[14] + c[14] + d[14];
  r[15] = a[15] + b[15] + c[15] + d[15];
  return r;
};

// Multivector subtraction (-)
GA3p ga3_sub(GA3 r, const GA3 a, const GA3 b)
{
  r[ 0] = a[ 0] - b[ 0];
  r[ 1] = a[ 1] - b[ 1];
  r[ 2] = a[ 2] - b[ 2];
  r[ 3] = a[ 3] - b[ 3];
  r[ 4] = a[ 4] - b[ 4];
  r[ 5] = a[ 5] - b[ 5];
  r[ 6] = a[ 6] - b[ 6];
  r[ 7] = a[ 7] - b[ 7];
  r[ 8] = a[ 8] - b[ 8];
  r[ 9] = a[ 9] - b[ 9];
  r[10] = a[10] - b[10];
  r[11] = a[11] - b[11];
  r[12] = a[12] - b[12];
  r[13] = a[13] - b[13];
  r[14] = a[14] - b[14];
  r[15] = a[15] - b[15];
  return r;
};

// Multivector/scalar multiplication (*)
GA3p ga3_muls(GA3 r, const GA3 a, const float b)
{
  r[ 0] = a[ 0]*b;
  r[ 1] = a[ 1]*b;
  r[ 2] = a[ 2]*b;
  r[ 3] = a[ 3]*b;
  r[ 4] = a[ 4]*b;
  r[ 5] = a[ 5]*b;
  r[ 6] = a[ 6]*b;
  r[ 7] = a[ 7]*b;
  r[ 8] = a[ 8]*b;
  r[ 9] = a[ 9]*b;
  r[10] = a[10]*b;
  r[11] = a[11]*b;
  r[12] = a[12]*b;
  r[13] = a[13]*b;
  r[14] = a[14]*b;
  r[15] = a[15]*b;
  return r;
};

// Scalar/multivector multiplication (*)
GA3p ga3_smul(GA3 r, const float a, const GA3 b)
{
  r[ 0] = a*b[ 0];
  r[ 1] = a*b[ 1];
  r[ 2] = a*b[ 2];
  r[ 3] = a*b[ 3];
  r[ 4] = a*b[ 4];
  r[ 5] = a*b[ 5];
  r[ 6] = a*b[ 6];
  r[ 7] = a*b[ 7];
  r[ 8] = a*b[ 8];
  r[ 9] = a*b[ 9];
  r[10] = a*b[10];
  r[11] = a*b[11];
  r[12] = a*b[12];
  r[13] = a*b[13];
  r[14] = a*b[14];
  r[15] = a*b[15];
  return r;
};

// Multivector/scalar addition (+)
GA3p ga3_adds(GA3 r, const GA3 a, const float b)
{
  r[ 0] = a[ 0] + b;
  r[ 1] = a[ 1];
  r[ 2] = a[ 2];
  r[ 3] = a[ 3];
  r[ 4] = a[ 4];
  r[ 5] = a[ 5];
  r[ 6] = a[ 6];
  r[ 7] = a[ 7];
  r[ 8] = a[ 8];
  r[ 9] = a[ 9];
  r[10] = a[10];
  r[11] = a[11];
  r[12] = a[12];
  r[13] = a[13];
  r[14] = a[14];
  r[15] = a[15];
  return r;
};

// Scalar/multivector addition (+)
GA3p ga3_sadd(GA3 r, const float a, const GA3 b)
{
  r[ 0] = a + b[ 0];
  r[ 1] =     b[ 1];
  r[ 2] =     b[ 2];
  r[ 3] =     b[ 3];
  r[ 4] =     b[ 4];
  r[ 5] =     b[ 5];
  r[ 6] =     b[ 6];
  r[ 7] =     b[ 7];
  r[ 8] =     b[ 8];
  r[ 9] =     b[ 9];
  r[10] =     b[10];
  r[11] =     b[11];
  r[12] =     b[12];
  r[13] =     b[13];
  r[14] =     b[14];
  r[15] =     b[15];
  return r;
};

// Multivector/scalar subtraction (-)
GA3p ga3_subs(GA3 r, const GA3 a, const float b)
{
  r[ 0] = a[ 0] - b;
  r[ 1] = a[ 1];
  r[ 2] = a[ 2];
  r[ 3] = a[ 3];
  r[ 4] = a[ 4];
  r[ 5] = a[ 5];
  r[ 6] = a[ 6];
  r[ 7] = a[ 7];
  r[ 8] = a[ 8];
  r[ 9] = a[ 9];
  r[10] = a[10];
  r[11] = a[11];
  r[12] = a[12];
  r[13] = a[13];
  r[14] = a[14];
  r[15] = a[15];
  return r;
};

// Scalar/multivector subtraction (-)
GA3p ga3_ssub(GA3 r, const float a, const GA3 b)
{
  r[ 0] = a - b[ 0];
  r[ 1] =   - b[ 1];
  r[ 2] =   - b[ 2];
  r[ 3] =   - b[ 3];
  r[ 4] =   - b[ 4];
  r[ 5] =   - b[ 5];
  r[ 6] =   - b[ 6];
  r[ 7] =   - b[ 7];
  r[ 8] =   - b[ 8];
  r[ 9] =   - b[ 9];
  r[10] =   - b[10];
  r[11] =   - b[11];
  r[12] =   - b[12];
  r[13] =   - b[13];
  r[14] =   - b[14];
  r[15] =   - b[15];
  return r;
};

#endif // _ga3_impl_
#endif // _ga3_h_