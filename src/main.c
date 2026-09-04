#include <stdint.h>
#include <stdbool.h>

#include "./includes/constants.h"
#define SDL_GFX_IMPLEMENTATION
#include "./includes/sdl_gfx.h"
#include "./includes/mesh.h"
#include "./includes/camera.h"
#include "./includes/matrix.h"
#include "./includes/inputs.h"
#include "includes/draw.h"
#include "includes/vectors.h"

#include "./sdl_gfx.c"
#include "./camera.c"
#include "./draw.c"
#include "./inputs.c"
#include "./matrix.c"
#include "./mesh.c"
#include "./vectors.c"

void transform_vertices(vec3_t* transformed, const vec3_t* original, const int count, const mat4x4_t* mat) {
    for (int i = 0; i < count; ++i) {
        transformed[i] = mat4_mul_vec3(mat, original[i]);
    }
}

int main()
{
  sdl_gfx *gfx = sdl_gfx_init("software renderer", SCREEN_WIDTH, SCREEN_HEIGHT);

  mesh_t cube        = create_cube();
  camera_t camera    = create_camera((vec3_t){0.0f, 0.0f, -3.0f});

  vec3_t translation = (vec3_t){0.0f, 0.0f, 0.0f};
  vec3_t rotation    = (vec3_t){0.0f, 0.0f, 0.0f};
  float scale        = 1.0;

  const int render_mode_count = 2;
  int render_mode = 0;

  const mat4x4_t perspective_mat = create_perspective_matrix(FOV, SCREEN_WIDTH, SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

  SDL_Event e;
  bool running = true;
  const uint8_t* KEYS = SDL_GetKeyboardState(NULL);
  Uint64 last_time = SDL_GetPerformanceCounter();
    
  while(running) {
    //
    // delta time stuff
    const Uint64 current_time = SDL_GetPerformanceCounter();
    const float delta_time    = (float)(current_time - last_time) / (float)SDL_GetPerformanceFrequency();
    last_time                 = current_time;

    //
    // handle inputs
    while(SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) { running = false; } }
    if (KEYS[SDL_SCANCODE_ESCAPE]) { running = 0; }
    handle_inputs(&translation, &rotation, &scale, &render_mode, render_mode_count, delta_time);

    mat4x4_t translation_mat = create_translation_matrix(translation.x, translation.y, translation.z);
    mat4x4_t rotation_mat    = create_rotation_matrix(rotation.x, rotation.y, rotation.z);
    mat4x4_t scale_mat       = create_scale_matrix(scale, scale, scale);
    mat4x4_t rot_mul_scale   = mat4_mul_mat4(&rotation_mat, &scale_mat);
    mat4x4_t model_mat       = mat4_mul_mat4(&translation_mat, &rot_mul_scale);
    mat4x4_t view_mat        = create_view_matrix(camera.position, camera.target);
    mat4x4_t mode_view_mat   = mat4_mul_mat4(&view_mat, &model_mat);

    transform_vertices(cube.transformed_vertices, cube.vertices, cube.vertex_count, &mode_view_mat);

    sdl_gfx_clear(gfx, 0x000000FF);

    switch (render_mode) {
      case 0:
        draw_wireframe(gfx, cube.transformed_vertices, cube.triangles, cube.triangles_count, 0xFFFF00FF, &perspective_mat, false);
    }

    sdl_gfx_render(gfx);
  }

  sdl_gfx_cleanup(gfx);
  return 0;
}
