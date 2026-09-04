#include <stdbool.h>

#define SDL_GFX_IMPLEMENTATION
#include "./sdl_gfx.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char** argv) {

  sdl_gfx* gfx = sdl_gfx_init("frame buffer", SCREEN_WIDTH, SCREEN_HEIGHT);

  bool isRunning = true;
  SDL_Event event;
  uint8_t colorOffset = 0;

  while (isRunning) {
    while (SDL_PollEvent(&event)) { if (event.type == SDL_QUIT) { isRunning = false; } }

    sdl_gfx_clear(gfx, 0xFFFF0000);

    // draw to CPU-side framebuffer pixel-by-pixel
    colorOffset++; // Animates the gradient pattern over time
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
      for (int x = 0; x < SCREEN_WIDTH; ++x) {

        // algorithm to calculate pixel colors
        uint8_t r = (uint8_t)(x * 255 / SCREEN_WIDTH + colorOffset);
        uint8_t g = (uint8_t)(y * 255 / SCREEN_HEIGHT);
        uint8_t b = (uint8_t)(255 - r);
        uint8_t a = 255; // Fully opaque

        // Pack channels into ARGB format (0xAARRGGBB)
        uint32_t pixelColor = (a << 24) | (r << 16) | (g << 8) | b;

        // Write pixel directly into the buffer array
        sdl_gfx_put_pixel(gfx, x, y, pixelColor);
      }
    }

    sdl_gfx_render(gfx);
  }

  sdl_gfx_cleanup(gfx);
  return 0;
}
