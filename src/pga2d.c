// 2D Projective Geometric Algebra
// Made from C++ code written by generator written by enki

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "pga2d.h"

// Norm
float pga2d_norm(const PGA2D a)
{
  PGA2D conj = {0}, mul = {0};
  return sqrtf(fabsf(
    pga2d_mul(
      mul,
      pga2d_conjugate(conj, a),
      a)
    [0]));
}

// Inverse norm
float pga2d_inorm(const PGA2D a)
{
  PGA2D dual = {0};
  pga2d_dual(dual, a);
  return pga2d_norm(dual);
}

// Normalize
PGA2Dp pga2d_normalized(PGA2D r, const PGA2D a)
{
  float n = 1 / pga2d_norm(a);
  return pga2d_muls(r, a, n);
}

// Reverse the order of the basis blades
PGA2Dp pga2d_reverse(PGA2D r, const PGA2D a)
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
PGA2Dp pga2d_dual(PGA2D r, const PGA2D a)
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
PGA2Dp pga2d_conjugate(PGA2D r, const PGA2D a)
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
PGA2Dp pga2d_involute(PGA2D r, const PGA2D a)
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
PGA2Dp pga2d_add(PGA2D r, const PGA2D a, const PGA2D b)
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

PGA2Dp pga2d_add3(PGA2D r, const PGA2D a, const PGA2D b, const PGA2D c)
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

// Multivector/scalar addition
PGA2Dp pga2d_adds(PGA2D r, const PGA2D a, const float b)
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
PGA2Dp pga2d_sub(PGA2D r, const PGA2D a, const PGA2D b)
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
PGA2Dp pga2d_subs(PGA2D r, const PGA2D a, const float b)
{
  r[0] = a[0] - b;
  r[1] = a[1];
  r[2] = a[2];
  r[3] = a[3];
  r[4] = a[4];
  r[5] = a[5];
  r[6] = a[6];
  r[7] = a[7];
  return r;
};

// The geometric product
PGA2Dp pga2d_mul(PGA2D r, const PGA2D a, const PGA2D b)
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

// Multivector/scalar multiplication
PGA2Dp pga2d_muls(PGA2D r, const PGA2D a, const float b)
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

// The outer product (MEET)
PGA2Dp pga2d_wedge(PGA2D r, const PGA2D a, const PGA2D b)
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
PGA2Dp pga2d_regressive(PGA2D r, const PGA2D a, const PGA2D b)
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
PGA2Dp pga2d_inner(PGA2D r, const PGA2D a, const PGA2D b)
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
