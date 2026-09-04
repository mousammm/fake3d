#include <stdbool.h>
#include "./includes/draw.h"
#include "includes/matrix.h"
#include "includes/mesh.h"
#include "includes/vectors.h"
#include "includes/constants.h"

static vec3_t project_to_screen(const mat4x4_t* mat, vec3_t v) {
  const vec4_t clip = mat4_mul_vec4(mat, (vec4_t){v.x, v.y, v.z, 1.0f});
  const float inv_w = 1.0f / clip.w;

  const float ndc_x = clip.x * inv_w;
  const float ndc_y = clip.y * inv_w;

  const float screen_x = ( ndc_x * 0.5f + 0.5f) * SCREEN_WIDTH;
  const float screen_y = (-ndc_y * 0.5f + 0.5f) * SCREEN_HEIGHT;

  return (vec3_t){screen_x, screen_y, inv_w};
}

static bool is_outside_frustum(const vec3_t p1, const vec3_t p2, const vec3_t p3) {
    if (p1.z > 1.0f || p2.z > 1.0f || p3.z > 1.0f || p1.z < -1.0f || p2.z < -1.0f || p3.z < -1.0f)
        return true;

    const float min_x = fminf(p1.x, fminf(p2.x, p3.x));
    const float max_x = fmaxf(p1.x, fmaxf(p2.x, p3.x));
    const float min_y = fminf(p1.y, fminf(p2.y, p3.y));
    const float max_y = fmaxf(p1.y, fmaxf(p2.y, p3.y));

    if (max_x < 0.0f || min_x > SCREEN_WIDTH || max_y < 0.0f || min_y > SCREEN_HEIGHT)
        return true;

    return false;
}

static void draw_line(sdl_gfx* gfx, const vec2_t a, const vec2_t b, const uint32_t color) {
    const float d_x = b.x - a.x;
    const float d_y = b.y - a.y;

    const float longer_delta = fabsf(d_x) >= fabsf(d_y) ? fabsf(d_x) : fabsf(d_y);

    const float inc_x = d_x / longer_delta;
    const float inc_y = d_y / longer_delta;

    float x = a.x;
    float y = a.y;

    for (int i = 0; i <= (int)longer_delta; ++i) {
        sdl_gfx_put_pixel(gfx, (int)x, (int)y, color);
        x += inc_x;
        y += inc_y;
    }
}

void draw_wireframe(
    sdl_gfx* gfx,
    const vec3_t* vertices,
    const triangle_t* triangles,
    int triangles_count,
    uint32_t color,
    const mat4x4_t* projection_matrix,
    bool cull_back_face)
{
  for (int i = 0; i < triangles_count; ++i) {
    const triangle_t triangle = triangles[i];

    const vec3_t v1 = vertices[triangle.v[0]];
    const vec3_t v2 = vertices[triangle.v[1]];
    const vec3_t v3 = vertices[triangle.v[2]];

    // backface todos

    const vec3_t p1 = project_to_screen(projection_matrix, v1);
    const vec3_t p2 = project_to_screen(projection_matrix, v2);
    const vec3_t p3 = project_to_screen(projection_matrix, v3);

    if (is_outside_frustum(p1, p2, p3))
      continue;

    draw_line(gfx, (vec2_t){p1.x, p1.y}, (vec2_t){p2.x, p2.y}, color);
    draw_line(gfx, (vec2_t){p2.x, p2.y}, (vec2_t){p3.x, p3.y}, color);
    draw_line(gfx, (vec2_t){p3.x, p3.y}, (vec2_t){p1.x, p1.y}, color);
    }
}
