#ifndef VECTORS_HEADER
#define VECTORS_HEADER

typedef struct {
  float x, y;
} vec2_t;

typedef struct {
  float x, y, z;
} vec3_t;

typedef struct {
  float x, y, z, w;
} vec4_t;

vec3_t vec3_normalize(vec3_t v);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
float  vec3_dot(vec3_t v1, vec3_t v2);

#endif // VECTORS_HEADER
