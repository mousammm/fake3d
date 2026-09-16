#ifndef SDL_GFX_HEADER
#define SDL_GFX_HEADER

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  int width;
  int height;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* frame_buffer_texture;
  uint32_t* frame_buffer;
} sh_gfx;

#ifndef SH_GFX_DEF
#define SH_GFX_DEF
#endif

//
// Core lib
SH_GFX_DEF sh_gfx* sh_gfx_init(const char* title, const int s_width, const int s_height);
SH_GFX_DEF void    sh_gfx_render(sh_gfx* gfx);
SH_GFX_DEF void    sh_gfx_clear(sh_gfx* gfx, const uint32_t color);
SH_GFX_DEF void    sh_gfx_put_pixel(sh_gfx* gfx, const int x, const int y, const uint32_t color);
SH_GFX_DEF void    sh_gfx_cleanup(sh_gfx* gfx);
SH_GFX_DEF void    sh_gfx_draw_line(sh_gfx* gfx, int x0, int y0, int x1, int y1, uint32_t color);
SH_GFX_DEF void    sh_gfx_draw_triangle(sh_gfx* gfx, int px1, int py1, int px2, int py2, int px3, int py3, uint32_t wfColor);
SH_GFX_DEF void    sh_gfx_fill_triangle(sh_gfx* gfx, int px1, int py1, int px2, int py2, int px3, int py3, uint32_t fillHeightColor);

#endif // SDL_GFX_HEADER

#ifdef SDL_GFX_IMPLEMENTATION

SH_GFX_DEF sh_gfx* sh_gfx_init(const char* title, const int s_width, const int s_height)
{
  sh_gfx* gfx = (sh_gfx*)malloc(sizeof(sh_gfx));

  SDL_Init(SDL_INIT_VIDEO);

  gfx->width = s_width;
  gfx->height = s_height;

  gfx->window                = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, s_width, s_height, 0);
  gfx->renderer              = SDL_CreateRenderer(gfx->window, -1, SDL_RENDERER_ACCELERATED);
  gfx->frame_buffer_texture  = SDL_CreateTexture(gfx->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, s_width, s_height);
  gfx->frame_buffer          = (uint32_t*)malloc(s_width * s_height* sizeof(uint32_t));

  return gfx;
}

SH_GFX_DEF void sh_gfx_render(sh_gfx* gfx)
{
  // push the CPU pixel array to the GPU streaming texture
  SDL_UpdateTexture(
    gfx->frame_buffer_texture, 
    NULL,                                // Update the whole texture
    gfx->frame_buffer,                    // Pointer to raw pixel buffer array
    gfx->width * sizeof(uint32_t)      // Pitch: size of one row of pixels in bytes
  );

  SDL_RenderClear(gfx->renderer);                                       // clear renderer
  SDL_RenderCopy(gfx->renderer, gfx->frame_buffer_texture, NULL, NULL); // copy texture to render
  SDL_RenderPresent(gfx->renderer);                                     // present to screen
  SDL_Delay(16);                                                        // cap to 60 fps
}

SH_GFX_DEF void sh_gfx_clear(sh_gfx* gfx, const uint32_t color)
{
  for (int i = 0; i < gfx->width * gfx->height; ++i) {
    gfx->frame_buffer[i] = color;
  }
}

SH_GFX_DEF void sh_gfx_put_pixel(sh_gfx* gfx, const int x, const int y, const uint32_t color)
{
  if (x < 0 || x >= gfx->width || y < 0 || y >= gfx->height)
    return;

  gfx->frame_buffer[y * gfx->width + x] = color;
}

SH_GFX_DEF void sh_gfx_cleanup(sh_gfx* gfx)
{
  free(gfx->frame_buffer);
  SDL_DestroyTexture(gfx->frame_buffer_texture);
  SDL_DestroyRenderer(gfx->renderer);
  SDL_DestroyWindow(gfx->window);
  SDL_Quit();
}

SH_GFX_DEF void sh_gfx_draw_line(sh_gfx* gfx, int x0, int y0, int x1, int y1, uint32_t color) {
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  
  int err = dx - dy;
  int e2;

  while (1) {
    sh_gfx_put_pixel(gfx, x0, y0, color);

    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

SH_GFX_DEF void sh_gfx_draw_triangle(sh_gfx* gfx, int px1, int py1, int px2, int py2, int px3, int py3, uint32_t wfColor)
{
  sh_gfx_draw_line(gfx, px1, py1, px2, py2, wfColor);
  sh_gfx_draw_line(gfx, px2, py2, px3, py3, wfColor);
  sh_gfx_draw_line(gfx, px3, py3, px1, py1, wfColor);
}

static void sh_gfx_swap_int(int* a, int* b) { int temp = *a; *a = *b; *b = temp; }
static void sh_gfx_draw_horizontal_line(sh_gfx* gfx, int x1, int x2, int y, uint32_t color) {
  if (x1 > x2) sh_gfx_swap_int(&x1, &x2);
  for (int x = x1; x <= x2; x++) { sh_gfx_put_pixel(gfx, x, y, color); }
}

SH_GFX_DEF void sh_gfx_fill_triangle(sh_gfx* gfx, int px1, int py1, int px2, int py2, int px3, int py3, uint32_t fillHeightColor)
{
  // 1. Sort vertices by Y-coordinate ascending (py1 <= py2 <= py3)
  if (py1 > py2) { sh_gfx_swap_int(&py1, &py2); sh_gfx_swap_int(&px1, &px2); }
  if (py1 > py3) { sh_gfx_swap_int(&py1, &py3); sh_gfx_swap_int(&px1, &px3); }
  if (py2 > py3) { sh_gfx_swap_int(&py2, &py3); sh_gfx_swap_int(&px2, &px3); }

  // Degenerate case: triangle has zero height
  if (py1 == py3) return;

  // 2. Check for flat-bottom, flat-top, or general triangle splitting
  if (py2 == py3) {
    // Flat-bottom triangle
    for (int y = py1; y <= py2; y++) {
      int xs = px1 + (y - py1) * (px2 - px1) / (py2 - py1);
      int xe = px1 + (y - py1) * (px3 - px1) / (py3 - py1);
      sh_gfx_draw_horizontal_line(gfx, xs, xe, y, fillHeightColor);
    }
  } 
  else if (py1 == py2) {
    // Flat-top triangle
    for (int y = py1; y <= py3; y++) {
      int xs = px1 + (y - py1) * (px3 - px1) / (py3 - py1);
      int xe = px2 + (y - py2) * (px3 - px2) / (py3 - py2);
      sh_gfx_draw_horizontal_line(gfx, xs, xe, y, fillHeightColor);
    }
  } 
  else {
    // General triangle: split into Flat-Bottom and Flat-Top
    // Find splitting point vertex (split_x, py2) on the long edge (px1,py1) -> (px3,py3)
    int split_x = px1 + (py2 - py1) * (px3 - px1) / (py3 - py1);

    // Top half (Flat-Bottom)
    for (int y = py1; y <= py2; y++) {
      int xs = px1 + (y - py1) * (px2 - px1) / (py2 - py1);
      int xe = px1 + (y - py1) * (split_x - px1) / (py2 - py1);
      sh_gfx_draw_horizontal_line(gfx, xs, xe, y, fillHeightColor);
    }

    // Bottom half (Flat-Top)
    for (int y = py2; y <= py3; y++) {
      int xs = px2 + (y - py2) * (px3 - px2) / (py3 - py2);
      int xe = split_x + (y - py2) * (px3 - split_x) / (py3 - py2);
      sh_gfx_draw_horizontal_line(gfx, xs, xe, y, fillHeightColor);
    }
  }
}

#endif // SDL_GFX_IMPLEMENTATION
