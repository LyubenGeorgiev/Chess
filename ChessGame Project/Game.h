#pragma once
#include "Board.h"
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
	void togglePromotionOptions();
	void init();

	void calculateBoardStates();

	bool isGameOver();
	void reset();
	~Game();

	void makeRandomMove();


};