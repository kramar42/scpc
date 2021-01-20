#include <stdio.h>
#include <math.h>

#include "ga3.h"

void print(const GA3 a)
{
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

// PGA points are trivectors.
// A point is just a homogeneous point, euclidean coordinates plus the origin
inline GA3p ga3_point(GA3 p, float x, float y, float z)
{
  GA3 a = {0}, b = {0}, c = {0};
  return ga3_add4(p,
    ga3_e123,
    ga3_smul(a,
      x, ga3_e032),
    ga3_smul(b,
      y, ga3_e013),
    ga3_smul(c,
      z, ga3_e021));
}

// PGA is plane based. Vectors are planes. (think linear functionals)
// A plane is defined using its homogenous equation ax + by + cz + d = 0
inline GA3p ga3_plane(GA3 p, float x, float y, float z, float w)
{
  GA3 a = {0}, b = {0}, c = {0}, d = {0};
  return ga3_add4(p,
    ga3_smul(a,
      x, ga3_e1),
    ga3_smul(b,
      y, ga3_e2),
    ga3_smul(c,
      z, ga3_e3),
    ga3_smul(d,
      w, ga3_e0));
}

// A rotor (Euclidean line) and translator (Ideal line)
inline GA3p ga3_rotor(GA3 r, const GA3 line, const float angle)
{
  GA3 a = {0}, b = {0};
  return ga3_sadd(r,
    cosf(angle),
    ga3_smul(a,
      sinf(angle),
      ga3_normalized(b,
        line)));
}

inline GA3p ga3_translator(GA3 t, GA3 line, float dist)
{
  GA3 a = {0};
  return ga3_sadd(t,
    1.0f,
    ga3_smul(a,
      dist / 2.0f,
      line));
}

inline GA3p ga3_transform(GA3 r, const GA3 trans, const GA3 elem)
{
  GA3 a = {0}, b = {0};
  return ga3_mul(r,
    ga3_mul(a,
      trans,
      elem),
    ga3_reverse(b,
      trans));
}

// Norm
inline float ga3_norm(const GA3 x)
{
  GA3 a = {0}, b = {0};
  return sqrtf(fabsf(
    ga3_mul(a,
      ga3_conjugate(b,
        x),
      x)
    [0]));
}

// Inverse norm
inline float ga3_inorm(const GA3 x)
{
  GA3 a = {0};
  return ga3_norm(
    ga3_dual(a,
      x));
}

// ???
inline float ga3_distance (const GA3 a, const GA3 b)
{
  GA3 d = {0};
  return ga3_norm(
    ga3_join(d,
      a, b));
}

// Normalize
inline GA3p ga3_normalized(GA3 r, const GA3 x)
{
  float n = 1 / ga3_norm(x);
  return ga3_muls(r,
    x, n);
}

// Reverse the order of the basis blades (~)
inline GA3p ga3_reverse(GA3 r, const GA3 a)
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
inline GA3p ga3_dual(GA3 r, const GA3 a)
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
inline GA3p ga3_conjugate(GA3 r, const GA3 a)
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
inline GA3p ga3_involute(GA3 r, const GA3 a)
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
inline GA3p ga3_mul(GA3 r, const GA3 a, const GA3 b)
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
inline GA3p ga3_meet(GA3 r, const GA3 a, const GA3 b)
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
inline GA3p ga3_join(GA3 r, const GA3 a, const GA3 b)
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
inline GA3p ga3_dot(GA3 r, const GA3 a, const GA3 b)
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
inline GA3p ga3_add(GA3 r, const GA3 a, const GA3 b)
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

inline GA3p ga3_add4(GA3 r, const GA3 a, const GA3 b, const GA3 c, const GA3 d)
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
inline GA3p ga3_sub(GA3 r, const GA3 a, const GA3 b)
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
inline GA3p ga3_muls(GA3 r, const GA3 a, const float b)
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
inline GA3p ga3_smul(GA3 r, const float a, const GA3 b)
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
inline GA3p ga3_adds(GA3 r, const GA3 a, const float b)
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
inline GA3p ga3_sadd(GA3 r, const float a, const GA3 b)
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
inline GA3p ga3_subs(GA3 r, const GA3 a, const float b)
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
inline GA3p ga3_ssub(GA3 r, const float a, const GA3 b)
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


// for our toy problem (generate points on the surface of a torus)
// we start with a function that generates motors.
// circle(t) with t going from 0 to 1.
// static GA3 circle(float t, float radius, GA3 line) {
  // return rotor(t*2.0f*PI,line) * translator(radius,ga3_e1*ga3_e0);
// }

// a torus is now the product of two circles.
// static GA3 torus(float s, float t, float r1, GA3 l1, float r2, GA3 l2) {
  // return circle(s,r2,l2)*circle(t,r1,l1);
// }

// and to sample its points we simply sandwich the origin ..
// static GA3 point_on_torus(float s, float t) {
  // GA3 to = torus(s,t,0.25f,ga3_e1*ga3_e2,0.6f,ga3_e1*ga3_e3);
  // return to * ga3_e123 * ~to;
// }

// Elements of the even subalgebra (scalar + bivector + pss) of unit length are motors
// GA3 rot = rotor( PI/2.0f, ga3_e1 * ga3_e2 );

// The outer product ^ is the MEET. Here we intersect the yz (x=0) and xz (y=0) planes.
// GA3 ax_z = ga3_e1 ^ ga3_e2;

// line and plane meet in point. We intersect the line along the z-axis (x=0,y=0) with the xy (z=0) plane.
// GA3 orig = ax_z ^ ga3_e3;

// We can also easily create points and join them into a line using the regressive (vee, &) product.
// GA3 px = point(1.0,0.0,0.0);
// GA3 line = orig & px;

// Lets also create the plane with equation 2x + z - 3 = 0
// GA3 p = plane(2,0,1,-3);

// rotations work on all elements
// GA3 rotated_plane = rot * p * ~rot;
// GA3 rotated_line  = rot * line * ~rot;
// GA3 rotated_point = rot * px * ~rot;

// See the 3D PGA Cheat sheet for a huge collection of useful formulas
// GA3 point_on_plane = (p | px) * p;

// Some output.
// printf("a point       : "); px.log();
// printf("a line        : "); line.log();
// printf("a plane       : "); p.log();
// printf("a rotor       : "); rot.log();
// printf("rotated line  : "); rotated_line.log();
// printf("rotated point : "); rotated_point.log();
// printf("rotated plane : "); rotated_plane.log();
// printf("point on plane: "); point_on_plane.normalized().log();
// printf("point on torus: "); point_on_torus(0.0f,0.0f).log();
// (ga3_e0-1.0f).log();
// (1.0f-ga3_e0).log();
