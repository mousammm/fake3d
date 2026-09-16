#include <math.h>
#include <stdbool.h>
#include "./constants.h"
#define SDL_GFX_IMPLEMENTATION
#include "./sh_gfx.h"
#include "./sh_la.h"

vec3d_t vCamera = {0};

int main(int argc, char** argv) {
  sh_gfx* gfx = sh_gfx_init("fake3d", SWIDTH, SHEIGHT);

  mesh_t cube      = create_cube();
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

    for (int i = 0; i < 12; ++i) {
      triangle_t tri = cube.tri[i];
      triangle_t triProjected, triTranslated, triRotated;

      triRotated.p[0] = multiply_mat4x4_vec3d(tri.p[0], matRot);
      triRotated.p[1] = multiply_mat4x4_vec3d(tri.p[1], matRot);
      triRotated.p[2] = multiply_mat4x4_vec3d(tri.p[2], matRot);

      triTranslated = triRotated;

      // PUSH THE CUBE IN Z DIR
      triTranslated.p[0].z = triRotated.p[0].z + 3.0f; // Push 3 units into screen
      triTranslated.p[1].z = triRotated.p[1].z + 3.0f;
      triTranslated.p[2].z = triRotated.p[2].z + 3.0f;

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
        // PROJECT 3D POINTS ONTO 2D VIEWPORT SPACE
        triProjected.p[0] = multiply_mat4x4_vec3d(triTranslated.p[0], matProj);
        triProjected.p[1] = multiply_mat4x4_vec3d(triTranslated.p[1], matProj);
        triProjected.p[2] = multiply_mat4x4_vec3d(triTranslated.p[2], matProj);

        // NORMALIZED DEVICE COORDINATES -> SCREEN SPACE
        for (int v = 0; v < 3; ++v) {
          // take the Coordinates and shift btw 0 and 2 
          // divide the 2 or 0.5 and scale it to appropriate size
          triProjected.p[v].x = (triProjected.p[v].x + 1.0f) * 0.5f * SWIDTH;
          triProjected.p[v].y = (triProjected.p[v].y + 1.0f) * 0.5f * SHEIGHT;
        }

        // DRAW THE CUBE
        uint32_t wfColor = 0xFFFFFFFF;
        sh_gfx_draw_line(gfx, triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y, wfColor);
        sh_gfx_draw_line(gfx, triProjected.p[1].x, triProjected.p[1].y, triProjected.p[2].x, triProjected.p[2].y, wfColor);
        sh_gfx_draw_line(gfx, triProjected.p[2].x, triProjected.p[2].y, triProjected.p[0].x, triProjected.p[0].y, wfColor);
      }
    }

    sh_gfx_render(gfx);
  }

  sh_gfx_cleanup(gfx);
  return 0;
}
