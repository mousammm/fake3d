#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include "./vectors.h"
#include <math.h>

#define DEG_TO_RAD 0.017453292519943f

typedef struct {
  float m[4][4];
} mat4x4_t;

vec3_t   mat4_mul_vec3(const mat4x4_t* mat, vec3_t vec);
vec4_t   mat4_mul_vec4(const mat4x4_t* mat, vec4_t vec);
mat4x4_t mat4_mul_mat4(const mat4x4_t* a, const mat4x4_t* b);

mat4x4_t create_translation_matrix(float x, float y, float z);
mat4x4_t create_scale_matrix(float sx, float sy, float sz);
mat4x4_t create_rotation_matrix(float pitch_X, float yaw_Y, float roll_Z);
mat4x4_t create_view_matrix(vec3_t eye, vec3_t target);
mat4x4_t create_perspective_matrix(float fov, int screen_width, int screen_height, float near, float far);

#endif // MATRIX_HEADER
