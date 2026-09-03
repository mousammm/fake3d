#include "./includes/camera.h"

camera_t create_camera(vec3_t position)
{
  return (camera_t) {
    .position = position,
    .target   = (vec3_t) {0.0f, 0.0f, -1.0f}
  };
}
