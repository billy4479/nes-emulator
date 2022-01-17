#define SDL_MAIN_HANDLED
#include "Application.hpp"

int main(int argc, char *argv[]) {
#ifdef DEBUG
    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
#endif

    (void)argc;
    (void)argv;

    Application app({800, 600});
    app.Run();

    return 0;
}
