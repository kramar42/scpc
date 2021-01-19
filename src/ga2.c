// 2D Projective Geometric Algebra
// Made from C++ code written by generator written by enki

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "ga2.h"

void ga2_print(const GA2 a)
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
        (i == 0) ? "" : ga2_basis[i]);
      j++;
    };
    if (n==0) printf("0");
    printf("\n");
}

// Constructors
GA2p ga2_point(GA2 p, const float x, const float y)
{
  GA2 a = {0}, b = {0};
  return ga2_add3(p,
    ga2_muls(a, ga2_e02, -x),
    ga2_muls(b, ga2_e01, y),
    ga2_e12);
}

GA2p ga2_line(GA2 l, const float x, const float y, const float z)
{
  GA2 a = {0}, b = {0}, c = {0};
  return ga2_add3(l,
    ga2_muls(a, ga2_e1, x),
    ga2_muls(b, ga2_e2, y),
    ga2_muls(c, ga2_e0, z));
}

GA2p ga2_rotor(GA2 r, const GA2 point, const float angle)
{
  GA2 a = {0};
  return ga2_adds(r,
    ga2_muls(a,
      ga2_normalized(a, point),
      sinf(angle)),
    cosf(angle));
}

GA2p ga2_translator(GA2 t, const GA2 point, const float dist)
{
  GA2 a = {0};
  return ga2_adds(t,
    ga2_muls(a,
      point,
      dist / 2.0f),
    1.0f);
}

GA2p ga2_transform(GA2 r, const GA2 trans, const GA2 elem)
{
  GA2 a = {0}, b = {0};
  return ga2_mul(r,
    ga2_mul(a,
      trans,
      elem),
    ga2_reverse(b, trans));
}

// Norm
float ga2_norm(const GA2 x)
{
  GA2 a = {0}, b = {0};
  return sqrtf(fabsf(
    ga2_mul(a,
      ga2_conjugate(b, x),
      x)
    [0]));
}

// Inverse norm
float ga2_inorm(const GA2 x)
{
  GA2 a = {0};
  return ga2_norm(
    ga2_dual(a, x));
}

float ga2_distance (const GA2 a, const GA2 b)
{
  GA2 d = {0};
  return ga2_norm(
    ga2_regressive(d,
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

// Multivector/scalar addition
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
GA2p ga2_subs(GA2 r, const GA2 a, const float b)
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

// Multivector/scalar multiplication
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

// The outer product (MEET)
GA2p ga2_wedge(GA2 r, const GA2 a, const GA2 b)
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
GA2p ga2_regressive(GA2 r, const GA2 a, const GA2 b)
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
