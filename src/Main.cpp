#include <cstdlib>
#include <ctime>

#include "Core/Game.hpp"

int main(int argc, char* argv[])
{
    std::srand(std::time(nullptr));

    Game game;
    game.start();
    return 0;
}
