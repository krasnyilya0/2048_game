#include "Game_2048.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>
using namespace std;

sf::Color getColor(int value) //Colors of the tiles
{
	switch (value) {
	case 2: return sf::Color(238, 228, 218);
	case 4: return sf::Color(237, 224, 200);
	case 8: return sf::Color(242, 177, 121);
	case 16: return sf::Color(245, 149, 99);
	case 32: return sf::Color(246, 124, 95);
	case 64: return sf::Color(246, 94, 59);
	case 128: return sf::Color(237, 207, 114);
	case 256: return sf::Color(237, 204, 97);
	case 512: return sf::Color(237, 200, 80);
	case 1024: return sf::Color(237, 197, 63);
	case 2048: return sf::Color(237, 194, 46);
	case 4096: return sf::Color(127, 255, 212);
	default: return sf::Color(205, 193, 180);
	}
}

Game::Game() {
	gameIsStillGoing = true;
	freeSlots = 16;
	restartBoard();
	
}

int Game::randomNumPlug() //Decides if the next number to plug in is 2 or 4
{
	return rand() % 10 <= 7 ? 2 : 4;
}

void Game::displayVisual(sf::RenderWindow& window) {
	window.clear(sf::Color::White);//Clears window
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {//Check if the font is there
		throw std::invalid_argument("No font found");
	}
	sf::RectangleShape tile(sf::Vector2f(100, 100));//Placing tiles across the board
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			tile.setPosition(j * 110+10, i * 110+10);
			tile.setFillColor(getColor(board[i][j]));
			window.draw(tile);

			if (board[i][j] != 0) {//Displays the number of the tile
				sf::Text text;
				text.setFont(font);
				text.setString(std::to_string(board[i][j]));
				text.setCharacterSize(24);
				text.setFillColor(sf::Color::Black);
				text.setPosition(j * 110 + 40, i * 110 + 40);
				window.draw(text);
			}
		}
	}
	window.display();
}





int Game::whereToPlugRandomNumber() {//Decides where to place the randomly chosen number
	if (freeSlots <= 0) {
		gameIsStillGoing = false;
		return 0;
	}
	return rand() % freeSlots;
}

void Game::restartBoard() {//Empty board
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			board[i][j] = 0;
		}
	}
	freeSlots = 16;
}

void Game::placeRandomNumber() {//Places the randomly chosen number between 2 and 4 and displays it at randomly generated location
	int location = whereToPlugRandomNumber();
	int value = randomNumPlug();
	int currentZero = 0;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (board[i][j] == 0) {
				if (currentZero == location) {
					board[i][j] = value;
					freeSlots--;
					return;
				}
				currentZero++;
			}
		}
	}
}

int Game::get(int x, int y, DIR dir) {//Detects input and interact with the new display of the board.
	switch (dir) {
	case UP: return board[3 - y][x];
	case DOWN: return board[y][3 - x];
	case LEFT: return board[x][3 - y];
	case RIGHT: return board[x][y];
	}
}

void Game::set(int x, int y, int val, DIR dir) {
	switch (dir) {
	case UP: board[3 - y][x] = val; break;
	case DOWN: board[y][3 - x] = val; break;
	case LEFT: board[x][3 - y] = val; break;
	case RIGHT: board[x][y] = val; break;
	}
}

bool Game::move(DIR dir) {
	bool moved = false;
	for (int i = 0; i < 4; ++i) {
		int target = 3;
		for (int j = 2; j >= 0; --j) {
			if (get(i, j, dir) != 0) {
				int k = j;
				while (k < target && get(i, k + 1, dir) == 0) {
					set(i, k + 1, get(i, k, dir), dir);
					set(i, k, 0, dir);
					k++;
					moved = true;
				}
				if (k < target && get(i, k + 1, dir) == get(i, k, dir)) {
					set(i, k + 1, get(i, k + 1, dir) * 2, dir);
					set(i, k, 0, dir);
					target = k;
					moved = true;
					freeSlots++;
				}
				else if (k != j) {
					moved = true;
				}
			}
		}
	}
	return moved;
}

bool Game::canMove() const {//Checks if the move is valid or not.
	if (freeSlots > 0) {
		return true;
	}
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if ((i > 0 && board[i][j] == board[i - 1][j]) ||
				(i < 3 && board[i][j] == board[i + 1][j]) ||
				(j > 0 && board[i][j] == board[i][j - 1]) ||
				(j < 3 && board[i][j] == board[i][j + 1])) {
				return true;
			}
		}
	}
	return false;
}

int Game::bestScore()  {//Displays Players best score
	int bestScoreNum = 0;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (board[i][j] > bestScoreNum) {
				bestScoreNum = board[i][j];
			}
		}
	}
	return bestScoreNum;
}

void Game::startGame(sf::RenderWindow& window)//Starts 2048 game
{
	gameIsStillGoing = true;
	placeRandomNumber();
	placeRandomNumber();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Keyboard::E) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				bool moved = false;
				if (event.key.code == sf::Keyboard::Left) {
					moved = move(LEFT);
				}
				else if (event.key.code == sf::Keyboard::Right) {
					moved = move(RIGHT);
				}
				else if (event.key.code == sf::Keyboard::Up) {
					moved = move(UP);
				}
				else if (event.key.code == sf::Keyboard::Down) {
					moved = move(DOWN);
				}

				if (moved) {
					placeRandomNumber();
				}
			}
		}

		if (!canMove()) {
			gameIsStillGoing = false;
			std::cout << "Game Over!" << '\n';
			std::cout << "Your best score was:" << bestScore()<< '\n';
			window.close();
		}

		displayVisual(window);
	}
}