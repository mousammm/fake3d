#ifndef SH_LA_A
#define SH_LA_A

#include <stdint.h>
#include <math.h>

#define DEG_TO_RAD 0.017453292519943f //  PI/180

typedef struct {
  double x, y, z;
} vec3d_t;

typedef struct {
  double m[4][4];
} mat4x4_t;

typedef struct {
  vec3d_t p[3];
  uint32_t color;
} triangle_t;

typedef struct {
  // triangle_t tri[12];
  triangle_t *tri;
  int triangle_count;
} mesh_t;

inline static vec3d_t multiply_mat4x4_vec3d(vec3d_t i, mat4x4_t m) {
  vec3d_t o;
      o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
      o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
      o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

  // normalize coordinates (convert from homogeneous coordinates to 3D space)
  if (w != 0.0f) { o.x /= w; o.y /= w; o.z /= w; }

  return o;
}

static mat4x4_t multiply_mat4x4(const mat4x4_t* a, const mat4x4_t* b)
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

static mat4x4_t create_rotation_matrix(float pitch_X, float yaw_Y, float roll_Z)
{
  // const float alpha = yaw_Y   * M_PI / 180.0f;
  // const float beta  = pitch_X * M_PI / 180.0f;
  // const float gamma = roll_Z  * M_PI / 180.0f;

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

static mat4x4_t create_perspective_matrix(int screen_width, int screen_height, float fov, float near, float far)
{
  // float fov_rad      = 1.0f / tanf(fov * 0.5f * M_PI / 180.0f);
  float fov_rad      = 1.0f / tanf(fov * 0.5f * DEG_TO_RAD);
  const float aspect = (float)screen_height / (float)screen_width;

  return (mat4x4_t) {{
    // {fov_rad/aspect,    0.0f,                       0.0f,  0.0f},
    {aspect*fov_rad,    0.0f,                       0.0f,  0.0f},
    {          0.0f, fov_rad,                       0.0f,  0.0f},
    {          0.0f,    0.0f,         far / (far - near),  1.0f},
    {          0.0f,    0.0f, (-far * near)/(far - near),  0.0f},
  }};
}

// inline static mesh_t create_cube()
// {
//   return (mesh_t) {
//     .tri = {
//       // SOUTH face
//       { .p = { {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0} } },
//       { .p = { {0.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 0.0} } },
//
//       // EAST face
//       { .p = { {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0} } },
//       { .p = { {1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {1.0, 0.0, 1.0} } },
//
//       // NORTH face
//       { .p = { {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0} } },
//       { .p = { {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 0.0, 1.0} } },
//
//       // WEST face
//       { .p = { {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 0.0} } },
//       { .p = { {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 0.0} } },
//
//       // TOP face
//       { .p = { {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 1.0, 1.0} } },
//       { .p = { {0.0, 1.0, 0.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 0.0} } },
//
//       // BOTTOM face
//       { .p = { {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0} } },
//       { .p = { {1.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0} } }
//     },
//   };
// }


#endif // SH_LA_A
