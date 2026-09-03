#ifndef MESH_HEADER
#define MESH_HEADER

#include "./vectors.h"

typedef struct {
  int  v[3];
  int uv[3];
  int  n[3];
} triangle_t;

typedef struct {
  vec3_t* transformed_vertices;
  vec3_t* transformed_normals;
  vec3_t* vertices;
  vec3_t* normals;
  vec2_t* uvs;
  triangle_t* triangles;

  int vertex_count;
  int normals_count;
  int uvs_count;
  int triangles_count;
} mesh_t;

mesh_t create_cube(void);

#endif // !MESH_HEADER
