// 2D Projective Geometric Algebra
// Made from C++ code written by generator written by enki

#ifndef _ga2_h_
#define _ga2_h_

typedef float  GA2[ 8];
typedef float* GA2p;

static const char* ga2_basis[] = { "1","e0","e1","e2","e01","e02","e12","e012" };


static const GA2 ga2_e0        = { 0, 1, 0, 0, 0, 0, 0, 0 };
static const GA2 ga2_e1        = { 0, 0, 1, 0, 0, 0, 0, 0 };
static const GA2 ga2_e2        = { 0, 0, 0, 1, 0, 0, 0, 0 };
static const GA2 ga2_e01       = { 0, 0, 0, 0, 1, 0, 0, 0 };
static const GA2 ga2_e02       = { 0, 0, 0, 0, 0, 1, 0, 0 };
static const GA2 ga2_e12       = { 0, 0, 0, 0, 0, 0, 1, 0 };
static const GA2 ga2_e012      = { 0, 0, 0, 0, 0, 0, 0, 1 };










void         ga2_print           (const GA2 a);

// constructors
GA2p         ga2_point           (GA2 p, const float x,   const float y);
GA2p         ga2_line            (GA2 l, const float x,   const float y,      const float z);
GA2p         ga2_rotor           (GA2 r, const GA2 point, const float angle);
GA2p         ga2_translator      (GA2 t, const GA2 point, const float dist);
GA2p         ga2_transform       (GA2 r, const GA2 trans, const GA2   elem);
GA2p         ga2_combine         (GA2 r, const GA2** trs, size_t n);
GA2p         ga2_circle          (GA2 r, const GA2 point, const float radius, const float t);


// norms
float        ga2_norm            (const GA2 x);
float        ga2_inorm           (const GA2 x);
float        ga2_distance        (const GA2 a, const GA2 b);

// unary
GA2p         ga2_normalized      (GA2 r, const GA2   a);
GA2p         ga2_reverse         (GA2 r, const GA2   a);
GA2p         ga2_dual            (GA2 r, const GA2   a);
GA2p         ga2_conjugate       (GA2 r, const GA2   a);
GA2p         ga2_involute        (GA2 r, const GA2   a);

// operators
GA2p         ga2_mul             (GA2 r, const GA2   a, const GA2   b);
GA2p         ga2_meet            (GA2 r, const GA2   a, const GA2   b);
GA2p         ga2_join            (GA2 r, const GA2   a, const GA2   b);
GA2p         ga2_dot             (GA2 r, const GA2   a, const GA2   b);
GA2p         ga2_add             (GA2 r, const GA2   a, const GA2   b);
GA2p         ga2_add3            (GA2 r, const GA2   a, const GA2   b, const GA2 c);
GA2p         ga2_sub             (GA2 r, const GA2   a, const GA2   b);
GA2p         ga2_muls            (GA2 r, const GA2   a, const float b);
GA2p         ga2_smul            (GA2 r, const float a, const GA2   b);
GA2p         ga2_adds            (GA2 r, const GA2   a, const float b);
GA2p         ga2_sadd            (GA2 r, const float a, const GA2   b);
GA2p         ga2_subs            (GA2 r, const GA2   a, const float b);
GA2p         ga2_ssub            (GA2 r, const float a, const GA2   b);



#ifdef _ga2_impl_

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

void ga2_print(const GA2 a)
{
  size_t n = 0;
  for (size_t i = 0, j = 0; i < 8; i++)
  {
    if (a[i] != 0.0f)
    {
      n++;
      printf("%s%0.7g%s",
        // sign between terms
        (j > 0) ? " + " : "",
        // coeff
        a[i],
        (i == 0) ? "" : ga2_basis[i]);
      j++;
    };
  }
  if (n == 0) printf("0");
  printf("\n");
}

// Constructors
GA2p ga2_point(GA2 p, const float x, const float y)
{
  GA2 a = {0}, b = {0};
  return ga2_add3(p,
    ga2_smul(a,
      -x, ga2_e02),
    ga2_smul(b,
       y, ga2_e01),
    ga2_e12);
}

GA2p ga2_line(GA2 l, const float x, const float y, const float z)
{
  GA2 a = {0}, b = {0}, c = {0};
  return ga2_add3(l,
    ga2_smul(a,
      x, ga2_e1),
    ga2_smul(b,
      y, ga2_e2),
    ga2_smul(c,
      z, ga2_e0));
}

GA2p ga2_rotor(GA2 r, const GA2 point, const float angle)
{
  GA2 a = {0};
  return ga2_sadd(r,
    cosf(angle),
    ga2_smul(a,
      sinf(angle),
      ga2_normalized(a,
        point)));
}

GA2p ga2_translator(GA2 t, const GA2 point, const float dist)
{
  GA2 a = {0};
  return ga2_sadd(t,
    1.0f,
    ga2_smul(a,
      dist / 2.0f,
      point));
}

GA2p ga2_transform(GA2 r, const GA2 trans, const GA2 elem)
{
  GA2 a = {0}, b = {0};
  return ga2_mul(r,
    ga2_mul(a,
      trans,
      elem),
    ga2_reverse(b,
      trans));
}

// circle(t) with t going from 0 to 1.
// TODO test
GA2p ga2_circle(GA2 r, const GA2 point, const float radius, const float t)
{
  GA2 a = {0}, b = {0}, c = {0};
  return ga2_mul(r,
    ga2_rotor(a,
      point,
      (float) (t * 2.0f * M_PI)),
    ga2_translator(b,
      ga2_mul(c,
        ga2_e1,
        ga2_e0),
      radius));
}

// Norm
float ga2_norm(const GA2 x)
{
  GA2 a = {0}, b = {0};
  return sqrtf(fabsf(
    ga2_mul(a,
      ga2_conjugate(b,
        x),
      x)
    [0]));
}

// Inverse norm
float ga2_inorm(const GA2 x)
{
  GA2 a = {0};
  return ga2_norm(
    ga2_dual(a,
      x));
}

float ga2_distance (const GA2 a, const GA2 b)
{
  GA2 d = {0};
  return ga2_norm(
    ga2_join(d,
      a, b));
}

// Normalize
GA2p ga2_normalized(GA2 r, const GA2 x)
{
  float n = 1 / ga2_norm(x);
  return ga2_muls(r,
    x, n);
}

// Reverse the order of the basis blades
GA2p ga2_reverse(GA2 r, const GA2 a)
{
  r[0] =  a[0];
  r[1] =  a[1];
  r[2] =  a[2];
  r[3] =  a[3];
  r[4] = -a[4];
  r[5] = -a[5];
  r[6] = -a[6];
  r[7] = -a[7];
  return r;
};

// Poincare duality operator
GA2p ga2_dual(GA2 r, const GA2 a)
{
  r[0] = a[7];
  r[1] = a[6];
  r[2] = a[5];
  r[3] = a[4];
  r[4] = a[3];
  r[5] = a[2];
  r[6] = a[1];
  r[7] = a[0];
  return r;
};

// Clifford conjugation
GA2p ga2_conjugate(GA2 r, const GA2 a)
{
  r[0]=  a[0];
  r[1]= -a[1];
  r[2]= -a[2];
  r[3]= -a[3];
  r[4]= -a[4];
  r[5]= -a[5];
  r[6]= -a[6];
  r[7]=  a[7];
  return r;
};

// Main involution
GA2p ga2_involute(GA2 r, const GA2 a)
{
  r[0] =  a[0];
  r[1] = -a[1];
  r[2] = -a[2];
  r[3] = -a[3];
  r[4] =  a[4];
  r[5] =  a[5];
  r[6] =  a[6];
  r[7] = -a[7];
  return r;
};

// Multivector addition
GA2p ga2_add(GA2 r, const GA2 a, const GA2 b)
{
  r[0] = a[0] + b[0];
  r[1] = a[1] + b[1];
  r[2] = a[2] + b[2];
  r[3] = a[3] + b[3];
  r[4] = a[4] + b[4];
  r[5] = a[5] + b[5];
  r[6] = a[6] + b[6];
  r[7] = a[7] + b[7];
  return r;
};

GA2p ga2_add3(GA2 r, const GA2 a, const GA2 b, const GA2 c)
{
  r[0] = a[0] + b[0] + c[0];
  r[1] = a[1] + b[1] + c[1];
  r[2] = a[2] + b[2] + c[2];
  r[3] = a[3] + b[3] + c[3];
  r[4] = a[4] + b[4] + c[4];
  r[5] = a[5] + b[5] + c[5];
  r[6] = a[6] + b[6] + c[6];
  r[7] = a[7] + b[7] + c[7];
  return r;
};

// Scalar/multivector addition (+)
GA2p ga2_sadd(GA2 r, const float a, const GA2 b)
{
  r[0] = a + b[0];
  r[1] =     b[1];
  r[2] =     b[2];
  r[3] =     b[3];
  r[4] =     b[4];
  r[5] =     b[5];
  r[6] =     b[6];
  r[7] =     b[7];
  return r;
};

// Multivector/scalar addition (+)
GA2p ga2_adds(GA2 r, const GA2 a, const float b)
{
  r[0] = a[0] + b;
  r[1] = a[1];
  r[2] = a[2];
  r[3] = a[3];
  r[4] = a[4];
  r[5] = a[5];
  r[6] = a[6];
  r[7] = a[7];
  return r;
};

// Multivector subtraction
GA2p ga2_sub(GA2 r, const GA2 a, const GA2 b)
{
  r[0] = a[0] - b[0];
  r[1] = a[1] - b[1];
  r[2] = a[2] - b[2];
  r[3] = a[3] - b[3];
  r[4] = a[4] - b[4];
  r[5] = a[5] - b[5];
  r[6] = a[6] - b[6];
  r[7] = a[7] - b[7];
  return r;
};

// Multivector/scalar subtraction
GA2p ga2_ssub(GA2 r, const float a, const GA2 b)
{
  r[0] = a - b[0];
  r[1] =   - b[1];
  r[2] =   - b[2];
  r[3] =   - b[3];
  r[4] =   - b[4];
  r[5] =   - b[5];
  r[6] =   - b[6];
  r[7] =   - b[7];
  return r;
};

// The geometric product
GA2p ga2_mul(GA2 r, const GA2 a, const GA2 b)
{
  r[0] = b[0]*a[0] + b[2]*a[2] + b[3]*a[3] - b[6]*a[6];
  r[1] = b[1]*a[0] + b[0]*a[1] - b[4]*a[2] - b[5]*a[3] + b[2]*a[4] + b[3]*a[5] - b[7]*a[6] - b[6]*a[7];
  r[2] = b[2]*a[0] + b[0]*a[2] - b[6]*a[3] + b[3]*a[6];
  r[3] = b[3]*a[0] + b[6]*a[2] + b[0]*a[3] - b[2]*a[6];
  r[4] = b[4]*a[0] + b[2]*a[1] - b[1]*a[2] + b[7]*a[3] + b[0]*a[4] - b[6]*a[5] + b[5]*a[6] + b[3]*a[7];
  r[5] = b[5]*a[0] + b[3]*a[1] - b[7]*a[2] - b[1]*a[3] + b[6]*a[4] + b[0]*a[5] - b[4]*a[6] - b[2]*a[7];
  r[6] = b[6]*a[0] + b[3]*a[2] - b[2]*a[3] + b[0]*a[6];
  r[7] = b[7]*a[0] + b[6]*a[1] - b[5]*a[2] + b[4]*a[3] + b[3]*a[4] - b[2]*a[5] + b[1]*a[6] + b[0]*a[7];
  return r;
};

// Multivector/scalar multiplication (*)
GA2p ga2_muls(GA2 r, const GA2 a, const float b)
{
  r[0] = a[0]*b;
  r[1] = a[1]*b;
  r[2] = a[2]*b;
  r[3] = a[3]*b;
  r[4] = a[4]*b;
  r[5] = a[5]*b;
  r[6] = a[6]*b;
  r[7] = a[7]*b;
  return r;
};

// Scalar/multivector multiplication (*)
GA2p ga2_smul(GA2 r, const float a, const GA2 b)
{
  r[0] = a*b[0];
  r[1] = a*b[1];
  r[2] = a*b[2];
  r[3] = a*b[3];
  r[4] = a*b[4];
  r[5] = a*b[5];
  r[6] = a*b[6];
  r[7] = a*b[7];
  return r;
};

// The outer product (^)
GA2p ga2_meet(GA2 r, const GA2 a, const GA2 b)
{
  r [0] = b[0]*a[0];
  r [1] = b[1]*a[0] + b[0]*a[1];
  r [2] = b[2]*a[0] + b[0]*a[2];
  r [3] = b[3]*a[0] + b[0]*a[3];
  r [4] = b[4]*a[0] + b[2]*a[1] - b[1]*a[2] + b[0]*a[4];
  r [5] = b[5]*a[0] + b[3]*a[1] - b[1]*a[3] + b[0]*a[5];
  r [6] = b[6]*a[0] + b[3]*a[2] - b[2]*a[3] + b[0]*a[6];
  r [7] = b[7]*a[0] + b[6]*a[1] - b[5]*a[2] + b[4]*a[3] + b[3]*a[4] - b[2]*a[5] + b[1]*a[6] + b[0]*a[7];
  return r;
};

// The regressive product (JOIN)
GA2p ga2_join(GA2 r, const GA2 a, const GA2 b)
{
  r[7] = a[7]*b[7];
  r[6] = a[6]*b[7] + a[7]*b[6];
  r[5] = a[5]*b[7] + a[7]*b[5];
  r[4] = a[4]*b[7] + a[7]*b[4];
  r[3] = a[3]*b[7] - a[5]*b[6] + a[6]*b[5] + a[7]*b[3];
  r[2] = a[2]*b[7] - a[4]*b[6] + a[6]*b[4] + a[7]*b[2];
  r[1] = a[1]*b[7] - a[4]*b[5] + a[5]*b[4] + a[7]*b[1];
  r[0] = a[0]*b[7] + a[1]*b[6] - a[2]*b[5] + a[3]*b[4] + a[4]*b[3] - a[5]*b[2] + a[6]*b[1] + a[7]*b[0];
  return r;
};

// The inner product
GA2p ga2_inner(GA2 r, const GA2 a, const GA2 b)
{
  r[0] = b[0]*a[0] + b[2]*a[2] + b[3]*a[3] - b[6]*a[6];
  r[1] = b[1]*a[0] + b[0]*a[1] - b[4]*a[2] - b[5]*a[3] + b[2]*a[4] + b[3]*a[5] - b[7]*a[6] - b[6]*a[7];
  r[2] = b[2]*a[0] + b[0]*a[2] - b[6]*a[3] + b[3]*a[6];
  r[3] = b[3]*a[0] + b[6]*a[2] + b[0]*a[3] - b[2]*a[6];
  r[4] = b[4]*a[0] + b[7]*a[3] + b[0]*a[4] + b[3]*a[7];
  r[5] = b[5]*a[0] - b[7]*a[2] + b[0]*a[5] - b[2]*a[7];
  r[6] = b[6]*a[0] + b[0]*a[6];
  r[7] = b[7]*a[0] + b[0]*a[7];
  return r;
};

#endif // _ga2_impl
#endif // _ga2_h_