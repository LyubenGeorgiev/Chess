#include "Game.h"
#include "Window.h"
int Game::boardTopLeftX;
int Game::boardTopLeftY;


Game::~Game() {
	delete(board);
}


void Game::calculateBoardStates() {
	board->calculateBoardStates();
}

void Game::render() {
	board->render(board->getBoardState());
}

void Game::update() {

}

void Game::makeRandomMove() {
	board->makeRandomMove(board->getBoardState());
}

void Game::handleMouseButtonDown(SDL_MouseButtonEvent& b) {
	board->handleMouseButtonDown(b, board->getBoardState());

}



void Game::init() {
	boardTopLeftX = boardTopLeftY = 0;
	board = new Board();
	board->init();
}

void Game::resize() {
	board->resize();

	boardTopLeftX = (Window::screenWidth - board->getWidth()) / 2;
	boardTopLeftY = (Window::screenHeight - board->getHeight()) / 2;

}

bool Game::isGameOver()
{
	return board->isGameOver(board->getBoardState());
}

void Game::reset() {
	board->reset();
}