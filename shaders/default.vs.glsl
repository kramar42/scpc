#version 330 core
layout (location = 0) in vec3 position;
out vec3 PositionColor;
uniform float focal, width, height, near, far;
uniform float trans[16];

// The geometric product (*)
float[16] ga3_mul(float r[16], const float a[16], const float b[16])
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
// Reverse the order of the basis blades (~)
float[16] ga3_reverse(float r[16], const float a[16])
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
float[16] ga3_transform(float r[16], float trans[16], float elem[16])
{
  float[16] a;
  float[16] b;
  return ga3_mul(r,
    ga3_mul(a,
      trans,
      elem),
    ga3_reverse(b,
      trans));
};
float[16] vec3_ga3(float r[16], vec3 a)
{
  r[ 0] = 0;
  r[ 1] = 0;
  r[ 2] = 0;
  r[ 3] = 0;
  r[ 4] = 0;
  r[ 5] = 0;
  r[ 6] = 0;
  r[ 7] = 0;
  r[ 8] = 0;
  r[ 9] = 0;
  r[10] = 0;
  r[11] = a.z;
  r[12] = a.y;
  r[13] = a.x;
  r[14] = 1;
  r[15] = 0;
  return r;
}
// returns vec4 in a clip space
vec4 ga3_vec4(float elem[16])
{
  vec3 view = vec3(elem[13], elem[12], elem[11]);
  return vec4(
    view.x * focal / width,
    view.y * focal / height,
    (view.z - near) / (far - near) * (far + near) - near,
    view.z);
};

void main()
{
  float[16]p,t;
  gl_Position = ga3_vec4(
    ga3_transform(t,
      trans,
      vec3_ga3(p, position)));
  PositionColor = gl_Position.xyz;
}
