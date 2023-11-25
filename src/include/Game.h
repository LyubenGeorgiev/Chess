#pragma once
#include "Board.h"


/**
 * \brief Helper class connecting Board and Window
 * \author L. Georgiev
 */
class Game
{
private:
	Board* board;

public:
	static int boardTopLeftX;
	static int boardTopLeftY;

	void update();
	void render();
	void resize();

	void handleMouseButtonDown(SDL_MouseButtonEvent& b);
	void init();

	void calculateBoardStates();

	bool isGameOver();
	void reset();
	~Game();

	void makeRandomMove();

};