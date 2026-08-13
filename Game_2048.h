#pragma once

#include <cstdlib>
#include <array>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <SFML/Graphics.hpp>

typedef enum _DIR {
    UP,
    DOWN,
    LEFT,
    RIGHT
} DIR;

class Game {
public:
    Game();
    int randomNumPlug();
    int whereToPlugRandomNumber();
    void restartBoard();
    void startGame(sf::RenderWindow& window);
    void displayVisual(sf::RenderWindow& window);
    void placeRandomNumber();
    bool canMove() const;
    int bestScore();

private:
    int get(int x, int y, DIR dir);
    void set(int x, int y, int val, DIR dir);
    bool move(DIR direction);

    bool gameIsStillGoing;
    int board[4][4];
    int freeSlots;
};