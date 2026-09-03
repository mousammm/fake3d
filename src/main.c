#include <stdio.h>
#include <stdbool.h>

#include "./includes/constants.h"
#include "./includes/sdl_gfx.h"

#include "./sdl_gfx.c"

int main()
{
  sdl_gfx *gfx = sdl_gfx_init("software renderer", SCREEN_WIDTH, SCREEN_HEIGHT);

  SDL_Event e;
  bool running = true;
  const bool* KEYS = SDL_GetKeyboardState(NULL);
    
  while(running) {

    while(SDL_PollEvent(&e)) { if (e.type == SDL_EVENT_QUIT) { running = false; } }
    if (KEYS[SDL_SCANCODE_ESCAPE]) { running = 0; }
    if (KEYS[SDL_SCANCODE_W]) { printf("w pressed\n"); }
    if (KEYS[SDL_SCANCODE_S]) { printf("s pressed\n"); }
    if (KEYS[SDL_SCANCODE_A]) { printf("a pressed\n"); }
    if (KEYS[SDL_SCANCODE_D]) { printf("d pressed\n"); }

    // start draw
    SDL_SetRenderDrawColor(gfx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(gfx->renderer);
    
    // draw
    SDL_SetRenderDrawColor(gfx->renderer, 255, 0, 0, 255);
    SDL_FRect rect = {100, 100, 200, 150};
    SDL_RenderFillRect(gfx->renderer, &rect);

    // end draw
    SDL_RenderPresent(gfx->renderer);
    SDL_Delay(16);
  }

  sdl_gfx_cleanup(gfx);
  return 0;
}
