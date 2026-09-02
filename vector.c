#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <stdint.h>

typedef struct {
  float x, y;
} vec2_t;

typedef struct {
  float x, y, z;
} vect3_t;

typedef struct {
  float x, y, z, w;
} vect4_t;

float fast_inverse_sqrt(const float x) {
    union {
        float f;
        int32_t i;
    } conv;

    conv.f = x;
    conv.i = 0x5f3759df - (conv.i >> 1);
    float y = conv.f;

    return y * (1.5f - 0.5f * x * y * y);
}

vect3_t vec3_normalize(vect3_t v)
{
  float length_sq = v.x * v.x + v.y * v.y + v.z * v.z;

  if (length_sq == 0) {
    return (vect3_t) { 0.0f, 0.0f, 0.0f };
  }

  float inv_length = fast_inverse_sqrt(length_sq);

  return (vect3_t) {
    .x = v.x * inv_length,
    .y = v.y * inv_length,
    .z = v.z * inv_length,
  };
}

vect3_t vec3_cross(const vect3_t v1, const vect3_t v2)
{
  return (vect3_t) {
    v1.y * v2.z - v1.z * v2.y,
    v1.z * v2.x - v1.x * v2.z,
    v1.x * v2.y - v1.y * v2.x
  };
}

float vec3_dot(const vect3_t v1, const vect3_t v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

#endif // VECTOR_HEADER
