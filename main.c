#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow( "Framebuffer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* framebufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    uint32_t* localFramebuffer = (uint32_t*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));

    bool isRunning = true;
    SDL_Event event;
    uint8_t colorOffset = 0;

    while (isRunning) {
        while (SDL_PollEvent(&event)) { if (event.type == SDL_QUIT) { isRunning = false; } }

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
                localFramebuffer[y * SCREEN_WIDTH + x] = pixelColor;
            }
        }

        // push the CPU pixel array to the GPU streaming texture
        SDL_UpdateTexture(
            framebufferTexture, 
            NULL,                                // Update the whole texture
            localFramebuffer,                    // Pointer to raw pixel buffer array
            SCREEN_WIDTH * sizeof(uint32_t)      // Pitch: size of one row of pixels in bytes
        );

        SDL_RenderClear(renderer);                                // clear renderer
        SDL_RenderCopy(renderer, framebufferTexture, NULL, NULL); // copy texture to render
        SDL_RenderPresent(renderer);                              // present to screen
        SDL_Delay(16);                                            // cap to 60 fps
    }

    // Cleanup resources
    free(localFramebuffer);
    SDL_DestroyTexture(framebufferTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
