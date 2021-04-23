#define SDL_MAIN_HANDLED
#include "Application.hpp"

int main(int argc, char *argv[]) {
    Application app(800, 600);
    app.Run();

    return 0;
}
