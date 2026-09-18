#include "./sh_la.h"

// VECTORS
static float fast_inverse_sqrt(const float x)
{
  union {
    float f;
    int32_t i;
  } conv;

  conv.f = x;
  conv.i = 0x5f3759df - (conv.i >> 1);
  float y = conv.f;

  return y * (1.5f - 0.5f * x * y * y);
}

vec3d_t vec3d_normalize(vec3d_t v)
{
  const float length_sq = v.x * v.x + v.y * v.y + v.z * v.z;

  if (length_sq == 0.0f) {
    return (vec3d_t) {0.0f, 0.0f, 0.0f};
  }

  const float inv_length = fast_inverse_sqrt(length_sq);

  return (vec3d_t) {
    .x = v.x * inv_length,
    .y = v.y * inv_length,
    .z = v.z * inv_length,
  };
}

vec3d_t vec3d_cross(vec3d_t v1, vec3d_t v2)
{
  return (vec3d_t) {
    .x = v1.y * v2.z - v1.z * v2.y,
    .y = v1.z * v2.x - v1.x * v2.z,
    .z = v1.x * v2.y - v1.y * v2.x
  };
}

float vec3d_dot(vec3d_t v1, vec3d_t v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// MATRICES
// vec3d_t mat4x4_mul_vec3d(vec3d_t vec, mat4x4_t mat)
// {
//   return (vec3d_t) {
//     mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.z + mat.m[0][3],
//     mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.z + mat.m[1][3],
//     mat.m[2][0] * vec.x + mat.m[2][1] * vec.y + mat.m[2][2] * vec.z + mat.m[2][3]
//   };
// }

vec3d_t mat4x4_mul_vec3d(vec3d_t i, mat4x4_t m)
{
  vec3d_t o;
      o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
      o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
      o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

  // normalize coordinates (convert from homogeneous coordinates to 3D space)
  if (w != 0.0f) { o.x /= w; o.y /= w; o.z /= w; }

  return o;
}

mat4x4_t mat4x4_mul_mat4x4(const mat4x4_t* a, const mat4x4_t* b)
{
  mat4x4_t result = {0};

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = a->m[i][0] * b->m[0][j] + 
                       a->m[i][1] * b->m[1][j] +
                       a->m[i][2] * b->m[2][j] + 
                       a->m[i][3] * b->m[3][j]; 
    }
  }

  return result;
}

/*
vec4_t mat4x4_mul_vec4d(const mat4x4_t* mat, vec4_t vec)
{
  return (vec4_t) {
    mat->m[0][0] * vec.x + mat->m[0][1] * vec.y + mat->m[0][2] * vec.z + mat->m[0][3] * vec.w,
    mat->m[1][0] * vec.x + mat->m[1][1] * vec.y + mat->m[1][2] * vec.z + mat->m[1][3] * vec.w,
    mat->m[2][0] * vec.x + mat->m[2][1] * vec.y + mat->m[2][2] * vec.z + mat->m[2][3] * vec.w,
    mat->m[3][0] * vec.x + mat->m[3][1] * vec.y + mat->m[3][2] * vec.z + mat->m[3][3] * vec.w
  };
}
*/

/*
mat4x4_t create_translation_matrix(float x, float y, float z)
{
  return (mat4x4_t) {{
    {1.0f, 0.0f, 0.0f,    x},
    {0.0f, 1.0f, 0.0f,    y},
    {0.0f, 0.0f, 1.0f,    z},
    {0.0f, 0.0f, 0.0f, 1.0f},
  }};
}
*/

/*
mat4x4_t create_scale_matrix(float sx, float sy, float sz)
{
  return (mat4x4_t) {{
    {  sx, 0.0f, 0.0f, 0.0f},
    {0.0f,   sy, 0.0f, 0.0f},
    {0.0f, 0.0f,   sz, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f},
  }};
}
*/

mat4x4_t create_rotation_matrix(float pitch_X, float yaw_Y, float roll_Z)
{
  const float alpha = yaw_Y   * DEG_TO_RAD;
  const float beta  = pitch_X * DEG_TO_RAD;
  const float gamma = roll_Z  * DEG_TO_RAD;

  const float ca = cosf(alpha);
  const float sa = sinf(alpha);
  const float cb = cosf(beta);
  const float sb = sinf(beta);
  const float cg = cosf(gamma);
  const float sg = sinf(gamma);

  return (mat4x4_t) {{
    { ca*cb, ca*sb*sg - sa*cg, ca*sb*cg + sa*sg, 0},
    { sa*cb, sa*sb*sg + ca*cg, sa*sb*cg - ca*sg, 0},
    {   -sb,            cb*sg,            cb*cg, 0},
    {     0,                0,                0, 1}
  }};
}

mat4x4_t create_perspective_matrix(int screen_width, int screen_height, float fov, float near, float far)
{
  float fov_rad      = 1.0f / tanf(fov * 0.5f * DEG_TO_RAD);
  const float aspect = (float)screen_height / (float)screen_width;

  return (mat4x4_t) {{
    {aspect*fov_rad,    0.0f,                       0.0f,  0.0f},
    {          0.0f, fov_rad,                       0.0f,  0.0f},
    {          0.0f,    0.0f,         far / (far - near),  1.0f},
    {          0.0f,    0.0f, (-far * near)/(far - near),  0.0f},
  }};
}

/*
mat4x4_t create_view_matrix(vec3_t eye, vec3_t target)
{
  const vec3_t forward = vec3_normalize((vec3_t){eye.x - target.x, eye.y - target.y, eye.z - target.z });
  const vec3_t right   = vec3_cross((vec3_t){0.0f, 1.0f, 0.0f}, forward);
  const vec3_t up      = vec3_cross(forward, right);

  return (mat4x4_t) {{
    {   right.x,   right.y,   right.z,   -vec3_dot(right, eye)},
    {      up.x,      up.y,      up.z,      -vec3_dot(up, eye)},
    { forward.x, forward.y, forward.z, -vec3_dot(forward, eye)},
    {      0.0f,      0.0f,      0.0f,                    1.0f}
  }};
}
*/
