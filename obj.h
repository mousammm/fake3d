#ifndef OBJ_HEADER
#define OBJ_HEADER

#include "./sh_la.h"

typedef struct {
  vec3d_t p[3];
  uint32_t color;
} triangle_t;

typedef struct {
  triangle_t *tri;
  int triangle_count;
} mesh_t;

#endif
