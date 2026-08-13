#include <SFML/Graphics.hpp>
#include "Game_2048.h"

int main() {
    Game game;
    sf::RenderWindow window(sf::VideoMode(450, 450), "2048 Game");

    game.startGame(window);

    return 0;
}