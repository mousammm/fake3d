#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./constants.h"
#define SDL_GFX_IMPLEMENTATION
#include "./sh_gfx.h"
#include "./sh_la.h"

vec3d_t vCamera = {0};

int compareTriangles(const void* a, const void* b) {
    const triangle_t* tri1 = (const triangle_t*)a;
    const triangle_t* tri2 = (const triangle_t*)b;

    // Calculate average Z depth for both triangles (using pre-projected translated Z coordinates)
    float z1 = (tri1->p[0].z + tri1->p[1].z + tri1->p[2].z) / 3.0f;
    float z2 = (tri2->p[0].z + tri2->p[1].z + tri2->p[2].z) / 3.0f;

    if (z1 > z2) return -1;
    if (z1 < z2) return 1;
    return 0;
}

uint32_t getColor(float dp) {
  // Clamp the dot product between 0.0f (shadow) and 1.0f (fully lit)
  float intensity = (dp < 0.0f) ? 0.0f : (dp > 1.0f ? 1.0f : dp);

  // Define your base color components (e.g., White: R=255, G=255, B=255)
  uint8_t baseR = 255;
  uint8_t baseG = 255;
  uint8_t baseB = 255;
  uint8_t alpha = 255; // Fully opaque

  // Apply light intensity to the color channels
  uint8_t r = (uint8_t)(baseR * intensity);
  uint8_t g = (uint8_t)(baseG * intensity);
  uint8_t b = (uint8_t)(baseB * intensity);

  // Pack them into ARGB format (0xAARRGGBB) using bitwise shifts
  uint32_t argbColor = (alpha << 24) | (r << 16) | (g << 8) | b;

  return argbColor;
}

mesh_t loadOBJFile(const char* filename) {
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

int main(int argc, char** argv) {
  sh_gfx* gfx = sh_gfx_init("fake3d", SWIDTH, SHEIGHT);

  // mesh_t cube      = create_cube();
  mesh_t cube      = loadOBJFile("./ship.obj");
  mat4x4_t matProj = create_perspective_matrix(SWIDTH, SHEIGHT, FOV, NEAR_PLANE, FAR_PLANE);
  float fTheta     = 0.0f; // Angle tracker for rotation

  int quit = false;
  SDL_Event event;
  const uint8_t* KEYS = SDL_GetKeyboardState(NULL);
  Uint64 last_time = SDL_GetPerformanceCounter();

  while (!quit) {
    // DELTA TIME
    const Uint64 current_time = SDL_GetPerformanceCounter();
    const float delta_time    = (float)(current_time - last_time) / (float)SDL_GetPerformanceFrequency();
    last_time                 = current_time;

    // HANDLE INPUTS
    while(SDL_PollEvent(&event)) { if (event.type == SDL_QUIT) { quit = true; } }
    if (KEYS[SDL_SCANCODE_ESCAPE]) { quit = true; }

    // START DRAWING
    sh_gfx_clear(gfx, 0xFF000000);

    fTheta         += 60.0f * delta_time;
    mat4x4_t matRot = create_rotation_matrix(0.0f, fTheta, fTheta);

    // Allocate a dynamic array to buffer the visible triangles for this frame
    int trianglesToRenderCount = 0;
    triangle_t* vecTrianglesToRaster = (triangle_t*)malloc(cube.triangle_count * sizeof(triangle_t));

    for (int i = 0; i < cube.triangle_count; ++i) {
      triangle_t tri = cube.tri[i];
      triangle_t triProjected, triTranslated, triRotated;

      triRotated.p[0] = multiply_mat4x4_vec3d(tri.p[0], matRot);
      triRotated.p[1] = multiply_mat4x4_vec3d(tri.p[1], matRot);
      triRotated.p[2] = multiply_mat4x4_vec3d(tri.p[2], matRot);

      triTranslated = triRotated;

      // PUSH THE CUBE IN Z DIR
      triTranslated.p[0].z = triRotated.p[0].z + 10.0f; // Push 3 units into screen
      triTranslated.p[1].z = triRotated.p[1].z + 10.0f;
      triTranslated.p[2].z = triRotated.p[2].z + 10.0f;

      // BACKFACE
      vec3d_t normal, line1, line2;
      line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
      line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
      line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

      line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
      line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
      line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

      // CORSS PRODUCT
      normal.x = line1.y * line2.z - line1.z * line2.y;
      normal.y = line1.z * line2.x - line1.x * line2.z;
      normal.z = line1.x * line2.y - line1.y * line2.x;
      
      float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
      normal.x /= l; normal.y /= l; normal.z /= l;

      // if (normal.z < 0) {
      if (normal.x * (triTranslated.p[0].x - vCamera.x) +
          normal.y * (triTranslated.p[0].y - vCamera.y) +
          normal.z * (triTranslated.p[0].z - vCamera.z) 
          < 0.0f) {
        // ILLUMINATION
        vec3d_t light_direction = { 0.0f, 0.0f, -1.0f };
        float l = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
        light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;

        float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;
        triTranslated.color = getColor(dp);

        // PROJECT 3D POINTS ONTO 2D VIEWPORT SPACE
        triProjected.p[0] = multiply_mat4x4_vec3d(triTranslated.p[0], matProj);
        triProjected.p[1] = multiply_mat4x4_vec3d(triTranslated.p[1], matProj);
        triProjected.p[2] = multiply_mat4x4_vec3d(triTranslated.p[2], matProj);
        triProjected.color = triTranslated.color;

        // NORMALIZED DEVICE COORDINATES -> SCREEN SPACE
        for (int v = 0; v < 3; ++v) {
          // take the Coordinates and shift btw 0 and 2 
          // divide the 2 or 0.5 and scale it to appropriate size
          triProjected.p[v].x = (triProjected.p[v].x + 1.0f) * 0.5f * SWIDTH;
          triProjected.p[v].y = (triProjected.p[v].y + 1.0f) * 0.5f * SHEIGHT;
        }

        // store triangle for sorting
        vecTrianglesToRaster[trianglesToRenderCount] = triProjected;
        trianglesToRenderCount++;
      }
    }

    // 2. Sort the accumulated triangles using the Painter's Algorithm
    qsort(vecTrianglesToRaster, trianglesToRenderCount, sizeof(triangle_t), compareTriangles);

    for (int i = 0; i < trianglesToRenderCount; i++) {
      triangle_t raster_triangle = vecTrianglesToRaster[i];
      sh_gfx_fill_triangle(gfx, raster_triangle.p[0].x, raster_triangle.p[0].y, raster_triangle.p[1].x, raster_triangle.p[1].y, raster_triangle.p[2].x, raster_triangle.p[2].y, raster_triangle.color);
      //sh_gfx_draw_triangle(gfx, raster_triangle.p[0].x, raster_triangle.p[0].y, raster_triangle.p[1].x, raster_triangle.p[1].y, raster_triangle.p[2].x, raster_triangle.p[2].y, 0xFFFF0000);
    }
    free(vecTrianglesToRaster);

    sh_gfx_render(gfx);
  }

  sh_gfx_cleanup(gfx);
  return 0;
}
