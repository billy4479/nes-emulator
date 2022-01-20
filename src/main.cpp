#define SDL_MAIN_HANDLED

#include <glm/ext/vector_int2.hpp>

#include "./Common/Costants.hpp"
#include "Application.hpp"

int main(int argc, char *argv[]) {
#ifdef DEBUG
    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
#endif

    (void)argc;
    (void)argv;

    Application app(EMULATOR_SIZE);
    app.Run();

    return 0;
}
