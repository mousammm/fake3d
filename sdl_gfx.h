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

#ifdef SDL_GFX_IMPLEMENTATION

sdl_gfx* sdl_gfx_init(const char* title, const int s_width, const int s_height)
{
  sdl_gfx* gfx = malloc(sizeof(sdl_gfx));

  SDL_Init(SDL_INIT_VIDEO);

  gfx->width = s_width;
  gfx->height = s_height;

  gfx->window                = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, s_width, s_height, 0);
  gfx->renderer              = SDL_CreateRenderer(gfx->window, -1, SDL_RENDERER_ACCELERATED);
  gfx->frame_buffer_texture  = SDL_CreateTexture(gfx->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, s_width, s_height);
  gfx->frame_buffer          = (uint32_t*)malloc(s_width * s_height* sizeof(uint32_t));

  return gfx;
}

void sdl_gfx_render(sdl_gfx* gfx)
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

void sdl_gfx_clear(sdl_gfx* gfx, const uint32_t color)
{
  for (int i = 0; i < gfx->width * gfx->height; ++i) {
    gfx->frame_buffer[i] = color;
  }
}

void sdl_gfx_put_pixel(sdl_gfx* gfx, const int x, const int y, const uint32_t color)
{
  if (x < 0 || x >= gfx->width || y < 0 || y >= gfx->height)
    return;

  gfx->frame_buffer[y * gfx->width + x] = color;
}

void sdl_gfx_cleanup(sdl_gfx* gfx)
{
  free(gfx->frame_buffer);
  SDL_DestroyTexture(gfx->frame_buffer_texture);
  SDL_DestroyRenderer(gfx->renderer);
  SDL_DestroyWindow(gfx->window);
  SDL_Quit();
}

#endif // SDL_GFX_IMPLEMENTATION
