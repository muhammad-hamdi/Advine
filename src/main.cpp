#include "core/Game.h"

int main() {
    Game game(1280, 720, "My Game");

    // Optionally, add objects and set up the scene after the game is created.
    game.Run();

    return 0;
}
