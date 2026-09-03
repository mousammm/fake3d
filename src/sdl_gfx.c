#include "./includes/sdl_gfx.h"

sdl_gfx* sdl_gfx_init(const char *title, const int width, const int height)
{
  sdl_gfx* gfx = malloc(sizeof(sdl_gfx));

  SDL_Init(SDL_INIT_VIDEO);

  gfx->window      = SDL_CreateWindow(title, width, height, 0);
  gfx->renderer    = SDL_CreateRenderer(gfx->window, NULL);
  gfx->texture     = SDL_CreateTexture(gfx->renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_STATIC, width, height);
  gfx->buffer      = malloc(width * height * 4);
  gfx->buffer_size = width * height;

  return gfx;
}

void sdl_gfx_render(sdl_gfx* gfx)
{
  SDL_UpdateTexture(gfx->texture, NULL, gfx->buffer, gfx->width * 4);
  SDL_RenderClear(gfx->renderer);
  SDL_RenderTexture(gfx->renderer, gfx->texture, NULL, NULL);
  SDL_RenderPresent(gfx->renderer);
}

void sdl_gfx_draw_pixel(sdl_gfx* gfx, const int x, const int y, const uint32_t color)
{
  if (x < 0 || x >= gfx->width || y < 0 || y >= gfx->height)
    return;

  gfx->buffer[y * gfx->width + x] = color;
}

void sdl_gfx_clear(sdl_gfx* gfx, const uint32_t color)
{
  for (int i = 0; i < gfx->width * gfx->height; ++i) {
    gfx->buffer[i] = color;
  }
}

void sdl_gfx_cleanup(sdl_gfx* gfx)
{
  free(gfx->buffer);
  SDL_DestroyTexture(gfx->texture);
  SDL_DestroyRenderer(gfx->renderer);
  SDL_DestroyWindow(gfx->window);
  SDL_Quit();
}
