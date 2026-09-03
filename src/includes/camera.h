#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "./vectors.h"

typedef struct {
  vec3_t position;
  vec3_t target;
} camera_t;

camera_t create_camera(vec3_t position);

#endif // !CAMERA_HEADER
