#ifndef SDL_GFX_HEADER
#define SDL_GFX_HEADER

#include <SDL3/SDL.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  const int     width;
  const int     height;
  SDL_Window*   window;
  SDL_Renderer* renderer;
  SDL_Texture*  texture;
  uint32_t*     buffer;
  int           buffer_size;
} sdl_gfx;

sdl_gfx* sdl_gfx_init(const char *title, const int width, const int height);
void sdl_gfx_render(sdl_gfx* gfx);
void sdl_gfx_draw_pixel(sdl_gfx* gfx, const int x, const int y, const uint32_t color);
void sdl_gfx_clear(sdl_gfx* gfx, const uint32_t color);
void sdl_gfx_cleanup(sdl_gfx* gfx);

#endif // SDL_GFX_HEADER
