// 2D Projective Geometric Algebra
// Made from C++ code written by generator written by enki

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "pga2d.h"

static PGA2Dp pga2d_new()
{
  return malloc(sizeof(PGA2D));
}

static void pga2d_print(const PGA2D a)
{
  size_t n = 0;
  for (size_t i = 0, j = 0; i < 8; i++)
    if (a[i] != 0.0f)
    {
      n++;
      printf("%s%0.7g%s",
        // sign between terms
        (j > 0) ? " + " : "",
        // coeff
        a[i],
        (i == 0) ? "" : pga2d_basis[i]);
      j++;
    };
    if (n==0) printf("0");
    printf("\n");
}

// Norm
static float pga2d_norm(const PGA2D a)
{
  PGA2Dp c = pga2d_conjugate(a);
  PGA2Dp m = pga2d_mul(a, c);
  float r = sqrtf(fabsf(m[0]));
  free(c); free(m);
  return r;
}

// Inverse norm
static float pga2d_inorm(const PGA2D a)
{
  PGA2Dp d = pga2d_dual(a);
  float r = pga2d_norm(d);
  free(d);
  return r;
}

// Normalize
static PGA2Dp pga2d_normalized(const PGA2D a)
{
  float n = 1 / pga2d_norm(a);
  return pga2d_muls(a, n);
}

// Reverse the order of the basis blades
static PGA2Dp pga2d_reverse(const PGA2D a)
{
  PGA2Dp res = pga2d_new();
  res[0] =  a[0];
  res[1] =  a[1];
  res[2] =  a[2];
  res[3] =  a[3];
  res[4] = -a[4];
  res[5] = -a[5];
  res[6] = -a[6];
  res[7] = -a[7];
  return res;
};

// Poincare duality operator
static PGA2Dp pga2d_dual(const PGA2D a)
{
  PGA2Dp res = pga2d_new();
  res[0] = a[7];
  res[1] = a[6];
  res[2] = a[5];
  res[3] = a[4];
  res[4] = a[3];
  res[5] = a[2];
  res[6] = a[1];
  res[7] = a[0];
  return res;
};

// Clifford conjugation
static PGA2Dp pga2d_conjugate(const PGA2D a)
{
  PGA2Dp res = pga2d_new();
  res[0]=  a[0];
  res[1]= -a[1];
  res[2]= -a[2];
  res[3]= -a[3];
  res[4]= -a[4];
  res[5]= -a[5];
  res[6]= -a[6];
  res[7]=  a[7];
  return res;
};

// Main involution
static PGA2Dp pga2d_involute(const PGA2D a)
{
  PGA2Dp res = pga2d_new();
  res[0] =  a[0];
  res[1] = -a[1];
  res[2] = -a[2];
  res[3] = -a[3];
  res[4] =  a[4];
  res[5] =  a[5];
  res[6] =  a[6];
  res[7] = -a[7];
  return res;
};

// Multivector addition
static PGA2Dp pga2d_add(const PGA2D a, const PGA2D b)
{
  PGA2Dp res = pga2d_new();
  res[0] = a[0] + b[0];
  res[1] = a[1] + b[1];
  res[2] = a[2] + b[2];
  res[3] = a[3] + b[3];
  res[4] = a[4] + b[4];
  res[5] = a[5] + b[5];
  res[6] = a[6] + b[6];
  res[7] = a[7] + b[7];
  return res;
};

// Multivector/scalar addition
static PGA2Dp pga2d_adds(const PGA2D a, const float b)
{
  PGA2Dp res = pga2d_new();
  res[0] = a[0] + b;
  res[1] = a[1];
  res[2] = a[2];
  res[3] = a[3];
  res[4] = a[4];
  res[5] = a[5];
  res[6] = a[6];
  res[7] = a[7];
  return res;
};

// Multivector subtraction
static PGA2Dp pga2d_sub(const PGA2D a, const PGA2D b)
{
  PGA2Dp res = pga2d_new();
  res[0] = a[0] - b[0];
  res[1] = a[1] - b[1];
  res[2] = a[2] - b[2];
  res[3] = a[3] - b[3];
  res[4] = a[4] - b[4];
  res[5] = a[5] - b[5];
  res[6] = a[6] - b[6];
  res[7] = a[7] - b[7];
  return res;
};

// Multivector/scalar subtraction
static PGA2Dp pga2d_subs(const PGA2D a, const float b)
{
  PGA2Dp res = pga2d_new();
  res[0] = a[0] - b;
  res[1] = a[1];
  res[2] = a[2];
  res[3] = a[3];
  res[4] = a[4];
  res[5] = a[5];
  res[6] = a[6];
  res[7] = a[7];
  return res;
};

// The geometric product
static PGA2Dp pga2d_mul(const PGA2D a, const PGA2D b)
{
  PGA2Dp res = pga2d_new();
  res [0] = b[0]*a[0] + b[2]*a[2] + b[3]*a[3] - b[6]*a[6];
  res [1] = b[1]*a[0] + b[0]*a[1] - b[4]*a[2] - b[5]*a[3] + b[2]*a[4] + b[3]*a[5] - b[7]*a[6] - b[6]*a[7];
  res [2] = b[2]*a[0] + b[0]*a[2] - b[6]*a[3] + b[3]*a[6];
  res [3] = b[3]*a[0] + b[6]*a[2] + b[0]*a[3] - b[2]*a[6];
  res [4] = b[4]*a[0] + b[2]*a[1] - b[1]*a[2] + b[7]*a[3] + b[0]*a[4] - b[6]*a[5] + b[5]*a[6] + b[3]*a[7];
  res [5] = b[5]*a[0] + b[3]*a[1] - b[7]*a[2] - b[1]*a[3] + b[6]*a[4] + b[0]*a[5] - b[4]*a[6] - b[2]*a[7];
  res [6] = b[6]*a[0] + b[3]*a[2] - b[2]*a[3] + b[0]*a[6];
  res [7] = b[7]*a[0] + b[6]*a[1] - b[5]*a[2] + b[4]*a[3] + b[3]*a[4] - b[2]*a[5] + b[1]*a[6] + b[0]*a[7];
  return res;
};

// Multivector/scalar multiplication
static PGA2Dp pga2d_muls(const PGA2D a, const float b)
{
  PGA2Dp res = pga2d_new();
  res[0] = a[0]*b;
  res[1] = a[1]*b;
  res[2] = a[2]*b;
  res[3] = a[3]*b;
  res[4] = a[4]*b;
  res[5] = a[5]*b;
  res[6] = a[6]*b;
  res[7] = a[7]*b;
  return res;
};

// The outer product (MEET)
static PGA2Dp pga2d_wedge(const PGA2D a, const PGA2D b)
{
  PGA2Dp res = pga2d_new();
  res [0] = b[0]*a[0];
  res [1] = b[1]*a[0] + b[0]*a[1];
  res [2] = b[2]*a[0] + b[0]*a[2];
  res [3] = b[3]*a[0] + b[0]*a[3];
  res [4] = b[4]*a[0] + b[2]*a[1] - b[1]*a[2] + b[0]*a[4];
  res [5] = b[5]*a[0] + b[3]*a[1] - b[1]*a[3] + b[0]*a[5];
  res [6] = b[6]*a[0] + b[3]*a[2] - b[2]*a[3] + b[0]*a[6];
  res [7] = b[7]*a[0] + b[6]*a[1] - b[5]*a[2] + b[4]*a[3] + b[3]*a[4] - b[2]*a[5] + b[1]*a[6] + b[0]*a[7];
  return res;
};

// The regressive product (JOIN)
static PGA2Dp pga2d_regressive(const PGA2D a, const PGA2D b)
{
  PGA2Dp res = pga2d_new();
  res[7] = a[7]*b[7];
  res[6] = a[6]*b[7] + a[7]*b[6];
  res[5] = a[5]*b[7] + a[7]*b[5];
  res[4] = a[4]*b[7] + a[7]*b[4];
  res[3] = a[3]*b[7] - a[5]*b[6] + a[6]*b[5] + a[7]*b[3];
  res[2] = a[2]*b[7] - a[4]*b[6] + a[6]*b[4] + a[7]*b[2];
  res[1] = a[1]*b[7] - a[4]*b[5] + a[5]*b[4] + a[7]*b[1];
  res[0] = a[0]*b[7] + a[1]*b[6] - a[2]*b[5] + a[3]*b[4] + a[4]*b[3] - a[5]*b[2] + a[6]*b[1] + a[7]*b[0];
  return res;
};

// The inner product
static PGA2Dp pga2d_inner(const PGA2D a, const PGA2D b)
{
  PGA2Dp res = pga2d_new();
  res[0] = b[0]*a[0] + b[2]*a[2] + b[3]*a[3] - b[6]*a[6];
  res[1] = b[1]*a[0] + b[0]*a[1] - b[4]*a[2] - b[5]*a[3] + b[2]*a[4] + b[3]*a[5] - b[7]*a[6] - b[6]*a[7];
  res[2] = b[2]*a[0] + b[0]*a[2] - b[6]*a[3] + b[3]*a[6];
  res[3] = b[3]*a[0] + b[6]*a[2] + b[0]*a[3] - b[2]*a[6];
  res[4] = b[4]*a[0] + b[7]*a[3] + b[0]*a[4] + b[3]*a[7];
  res[5] = b[5]*a[0] - b[7]*a[2] + b[0]*a[5] - b[2]*a[7];
  res[6] = b[6]*a[0] + b[0]*a[6];
  res[7] = b[7]*a[0] + b[0]*a[7];
  return res;
};
