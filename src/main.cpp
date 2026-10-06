#include "Game.h"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "An error occurred: " << ex.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred.\n";
        return 1;
    }

    return 0;
}
