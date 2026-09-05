#ifndef DRAW_HEADER
#define DRAW_HEADER

#include "./sdl_gfx.h"
#include "./vectors.h"
#include "./mesh.h"
#include "./matrix.h"
#include <stdint.h>

void draw_wireframe(
    sdl_gfx* gfx,
    const vec3_t* vertices,
    const triangle_t* triangles,
    int triangles_count,
    uint32_t color,
    const mat4x4_t* perspective_projection_matrix,
    bool cull_back_face);

#endif // !DRAW_HEADER
