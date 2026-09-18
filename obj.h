#ifndef OBJ_HEADER
#define OBJ_HEADER

#include "./sh_la.h"
#include <stdio.h>

typedef struct {
  vec3d_t p[3];
  uint32_t color;
} triangle_t;

typedef struct {
  triangle_t *tri;
  int triangle_count;
} mesh_t;

static mesh_t loadOBJFile(const char* filename) {
  mesh_t mesh = { .tri = NULL, .triangle_count = 0 };
  FILE* file = fopen(filename, "r");
  if (!file) {
    printf("Error: Could not open file %s\n", filename);
    return mesh;
  }

  char line[128];
  int vertex_count = 0;
  int face_count = 0;

  // Pass 1: Count vertices and faces for memory allocation
  while (fgets(line, sizeof(line), file)) {
    if (line[0] == 'v' && line[1] == ' ') {
      vertex_count++;
    } else if (line[0] == 'f' && line[1] == ' ') {
      face_count++;
    }
  }

  // Allocate array to hold temporary vertices
  vec3d_t* verts = (vec3d_t*)malloc(vertex_count * sizeof(vec3d_t));
  // Allocate the actual triangles into your mesh struct
  mesh.tri = (triangle_t*)malloc(face_count * sizeof(triangle_t));
  mesh.triangle_count = face_count;

  // Rewind file to start parsing actual data
  rewind(file);

  int v_idx = 0;
  int f_idx = 0;

  while (fgets(line, sizeof(line), file)) {
    // Parse individual Vertex
    if (line[0] == 'v' && line[1] == ' ') {
      sscanf(line, "v %lf %lf %lf", &verts[v_idx].x, &verts[v_idx].y, &verts[v_idx].z);
      v_idx++;
    }
    // Parse individual Face (Triangle)
    else if (line[0] == 'f' && line[1] == ' ') {
      int f1, f2, f3;
      // Note: OBJ indices are 1-based, C arrays are 0-based (hence the -1)
      sscanf(line, "f %d %d %d", &f1, &f2, &f3);
      
      mesh.tri[f_idx].p[0] = verts[f1 - 1];
      mesh.tri[f_idx].p[1] = verts[f2 - 1];
      mesh.tri[f_idx].p[2] = verts[f3 - 1];
      mesh.tri[f_idx].color = 0xFFFFFFFF; // Default to fully opaque white ARGB
      
      f_idx++;
    }
  }

  fclose(file);
  free(verts); // Free temporary vertex array
  return mesh;
}

#endif
