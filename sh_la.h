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

// VECTORS
vec3d_t vec3d_normalize(vec3d_t v);
vec3d_t vec3d_cross(vec3d_t v1, vec3d_t v2);
float   vec3d_dot(vec3d_t v1, vec3d_t v2);

// MATRICES
vec3d_t  mat4x4_mul_vec3d(vec3d_t i, mat4x4_t m);
mat4x4_t mat4x4_mul_mat4x4(const mat4x4_t* a, const mat4x4_t* b);
//vec4_t mat4x4_mul_vec4d(const mat4x4_t* mat, vec4_t vec);
//mat4x4_t create_translation_matrix(float x, float y, float z);
//mat4x4_t create_scale_matrix(float sx, float sy, float sz);
mat4x4_t create_rotation_matrix(float pitch_X, float yaw_Y, float roll_Z);
mat4x4_t create_perspective_matrix(int screen_width, int screen_height, float fov, float near, float far);
// mat4x4_t create_view_matrix(vec3_t eye, vec3_t target);

#endif // SH_LA_A
