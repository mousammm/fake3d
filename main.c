#include <stdio.h>
#include <stdbool.h>
#include "constants.h"
#include "sdl_gfx.c"

int main()
{
  sdl_init("software renderer", SCREEN_WIDTH, SCREEN_HEIGHT);

  SDL_Event e;
  bool running = true;
  const Uint8* KEYS = SDL_GetKeyboardState(NULL);
    
  while(running) {

    while(SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) { running = false; } }
    if (KEYS[SDL_SCANCODE_ESCAPE]) { running = 0; }
    if (KEYS[SDL_SCANCODE_W]) { printf("w pressed\n"); }
    if (KEYS[SDL_SCANCODE_S]) { printf("s pressed\n"); }
    if (KEYS[SDL_SCANCODE_A]) { printf("a pressed\n"); }
    if (KEYS[SDL_SCANCODE_D]) { printf("d pressed\n"); }

    sdl_draw_start();
    
    // draw
    SDL_SetRenderDrawColor(pren, 255, 0, 0, 255);
    SDL_Rect rect = {100, 100, 200, 150};
    SDL_RenderFillRect(pren, &rect);
    
    sdl_draw_end();
  }

  sdl_cleanup();
  return 0;
}
