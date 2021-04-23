#define SDL_MAIN_HANDLED
#include "Application.hpp"

int main(int argc, char *argv[]) {
    // SDL_Init(SDL_INIT_VIDEO);

    // SDL_Window *window =
    //     SDL_CreateWindow("NES-Emulator", SDL_WINDOWPOS_UNDEFINED,
    //                      SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

    // SDL_Renderer *renderer =
    //     SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    // SDL_SetRenderDrawColor(renderer, 43, 43, 43, SDL_ALPHA_OPAQUE);
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);

    // SDL_Delay(3000);

    // SDL_DestroyWindow(window);
    // SDL_Quit();

    Application app(800, 600);
    app.Run();

    return 0;
}
