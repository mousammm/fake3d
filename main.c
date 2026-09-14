#include "./constants.h"
#define SDL_GFX_IMPLEMENTATION
#include "./sh_gfx.h"
#include "./sh_la.h"

int main(int argc, char** argv) {
  sh_gfx* gfx = sh_gfx_init("fake3d", SWIDTH, SHEIGHT);

  int isRunning = 1;
  SDL_Event event;

  mesh_t cube      = create_cube();
  mat4x4_t matProj = create_perspective_matrix(SWIDTH, SHEIGHT, FOV, NEAR_PLANE, FAR_PLANE);
  float fTheta     = 0.0f; // Angle tracker for rotation

  while (isRunning) {
    while (SDL_PollEvent(&event)) { if (event.type == SDL_QUIT) { isRunning = 0; } }

    sh_gfx_clear(gfx, 0xFF000000);

    fTheta += 0.02;

    // Setup Rotation Matrices
    mat4x4_t matRotZ = {0}, matRotX = {0};

    // Rotation Z
    matRotZ.m[0][0] = cosf(fTheta);  matRotZ.m[0][1] = sinf(fTheta);
    matRotZ.m[1][0] = -sinf(fTheta); matRotZ.m[1][1] = cosf(fTheta);
    matRotZ.m[2][2] = 1.0f;          matRotZ.m[3][3] = 1.0f;
    // Rotation X
    matRotX.m[0][0] = 1.0f;
    matRotX.m[1][1] = cosf(fTheta * 0.5f);  matRotX.m[1][2] = sinf(fTheta * 0.5f);
    matRotX.m[2][1] = -sinf(fTheta * 0.5f); matRotX.m[2][2] = cosf(fTheta * 0.5f);
    matRotX.m[3][3] = 1.0f;

    for (int i = 0; i < 12; ++i) {
      triangle_t tri = cube.tri[i];
      triangle_t triRotatedZ, triRotatedZX, triTranslated, triProjected;

      // Rotate in Z-Axis
      triRotatedZ.p[0] = multiply_mat4x4_vec3d(tri.p[0], matRotZ);
      triRotatedZ.p[1] = multiply_mat4x4_vec3d(tri.p[1], matRotZ);
      triRotatedZ.p[2] = multiply_mat4x4_vec3d(tri.p[2], matRotZ);

      // Rotate in X-Axis
      triRotatedZX.p[0] = multiply_mat4x4_vec3d(triRotatedZ.p[0], matRotX);
      triRotatedZX.p[1] = multiply_mat4x4_vec3d(triRotatedZ.p[1], matRotX);
      triRotatedZX.p[2] = multiply_mat4x4_vec3d(triRotatedZ.p[2], matRotX);

      // Translate geometry out into the scene (away from the camera)
      triTranslated = triRotatedZX;
      triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f; // Push 3 units into screen
      triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
      triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

      // Project 3D points onto 2D viewport space
      triProjected.p[0] = multiply_mat4x4_vec3d(triTranslated.p[0], matProj);
      triProjected.p[1] = multiply_mat4x4_vec3d(triTranslated.p[1], matProj);
      triProjected.p[2] = multiply_mat4x4_vec3d(triTranslated.p[2], matProj);

      // Scale and offset into screen pixel space (Normalized Device Coordinates -> Screen Space)
      // Map X and Y from [-1, 1] to [0, SWIDTH] and [0, SHEIGHT]
      for (int v = 0; v < 3; ++v) {
        triProjected.p[v].x = (triProjected.p[v].x + 1.0f) * 0.5f * SWIDTH;
        triProjected.p[v].y = (triProjected.p[v].y + 1.0f) * 0.5f * SHEIGHT;
      }

      uint32_t wireframeColor = 0xFFFFFFFF;
      sh_gfx_draw_line(gfx, triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y, 0xFFFFFF00);
      sh_gfx_draw_line(gfx, triProjected.p[1].x, triProjected.p[1].y, triProjected.p[2].x, triProjected.p[2].y, 0xFFFF0000);
      sh_gfx_draw_line(gfx, triProjected.p[2].x, triProjected.p[2].y, triProjected.p[0].x, triProjected.p[0].y, 0xFF0000FF);
    }

    sh_gfx_render(gfx);
  }

  sh_gfx_cleanup(gfx);
  return 0;
}
