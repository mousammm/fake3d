#ifndef SDL_GFX_HEADER
#define SDL_GFX_HEADER

/*
EXAMPLE USAGE:

  #define SDL_GFX_IMPLEMENTATION
  #include "./sdl_gfx.h"
  
  int main(int argc, char** argv) {
  
    sdl_gfx* gfx = sdl_gfx_init("frame buffer", 800, 600);
  
    int isRunning = 1;
    SDL_Event event;
  
    while (isRunning) {
      while (SDL_PollEvent(&event)) { if (event.type == SDL_QUIT) { isRunning = 0; } }
  
      sdl_gfx_clear(gfx, 0xFF000000); // 0xAARRGGBB
      sdl_gfx_put_pixel(gfx, x, y, pixelColor);
      sdl_gfx_render(gfx);
    }
  
    sdl_gfx_cleanup(gfx);
    return 0;
  }

*/

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
} sdl_gfx;

// argb frame buffer
sdl_gfx* sdl_gfx_init(const char* title, const int s_width, const int s_height);
void     sdl_gfx_render(sdl_gfx* gfx);
void     sdl_gfx_clear(sdl_gfx* gfx, const uint32_t color);
void     sdl_gfx_put_pixel(sdl_gfx* gfx, const int x, const int y, const uint32_t color);
void     sdl_gfx_cleanup(sdl_gfx* gfx);

#endif // !SDL_GFX_HEADER
