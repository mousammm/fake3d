#ifndef _SDL_GFX_
#define _SDL_GFX_

#include <SDL2/SDL.h>
SDL_Window *pwin;
SDL_Renderer *pren;

void sdl_init(const char *title, const int width, const int height)
{
  SDL_Init(SDL_INIT_VIDEO);
  pwin = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
  pren = SDL_CreateRenderer(pwin, -1, SDL_RENDERER_ACCELERATED);
}

void sdl_draw_start()
{
  SDL_SetRenderDrawColor(pren, 0, 0, 0, 255);
  SDL_RenderClear(pren);
}

void sdl_draw_end()
{
  SDL_RenderPresent(pren);
  SDL_Delay(16);
}

void sdl_cleanup()
{
  SDL_DestroyRenderer(pren);
  SDL_DestroyWindow(pwin);
  SDL_Quit();
}

#endif /* ifndef _SDL_GFX_ */
