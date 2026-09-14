#ifndef SH_LA_A
#define SH_LA_A

#include <math.h>

typedef struct {
  double x, y, z;
} vec3d_t;

typedef struct {
  double m[4][4];
} mat4x4_t;

typedef struct {
  vec3d_t p[3];
} triangle_t;

typedef struct {
  triangle_t tri[12];
} mesh_t;

inline static vec3d_t multiply_mat4x4_vec3d(vec3d_t i, mat4x4_t m) {
  vec3d_t o;
      o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
      o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
      o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

  // Perspective divide (convert from homogeneous coordinates to 3D space)
  // normalize coordinates
  if (w != 0.0f) { o.x /= w; o.y /= w; o.z /= w; }

  return o;
}

static mat4x4_t create_perspective_matrix(int screen_width, int screen_height, float fov, float near, float far)
{
  // fov deg to rad
  float fov_rad      = 1.0f / tanf(fov * 0.5f / 180.0f * M_PI);
  const float aspect = (float)screen_width / (float)screen_height;

  return (mat4x4_t) {{
    {fov_rad/aspect,    0.0f,                       0.0f,  0.0f},
    {          0.0f, fov_rad,                       0.0f,  0.0f},
    {          0.0f,    0.0f,         far / (far - near),  1.0f},
    {          0.0f,    0.0f, (-far * near)/(far - near),  0.0f},
  }};
}

inline static mesh_t create_cube()
{
  return (mesh_t) {
    .tri = {
      // SOUTH face
      { .p = { {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0} } },
      { .p = { {0.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 0.0} } },

      // EAST face
      { .p = { {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0} } },
      { .p = { {1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {1.0, 0.0, 1.0} } },

      // NORTH face
      { .p = { {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0} } },
      { .p = { {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 0.0, 1.0} } },

      // WEST face
      { .p = { {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 0.0} } },
      { .p = { {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 0.0} } },

      // TOP face
      { .p = { {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 1.0, 1.0} } },
      { .p = { {0.0, 1.0, 0.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 0.0} } },

      // BOTTOM face
      { .p = { {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0} } },
      { .p = { {1.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0} } }
    },
  };
}

#endif // SH_LA_A
