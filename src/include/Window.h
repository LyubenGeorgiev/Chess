#pragma once

#undef main

#include "Game.h"


#define MIN_WINDOW_WIDTH 400
#define MIN_WINDOW_HEIGHT 400

//defines for the default background color fo the window.
#define BACKGROUND_RED 122
#define BACKGROUND_GREEN 122
#define BACKGROUND_BLUE 122

/**
 * \brief Class for the window and handling events
 * \author L. Georgiev
 */
class Window
{
public:
	void update(float deltaTime);
	void handleEvents();
	void clean();
	void render();
	void run();

	bool running();

	~Window();
	Window();
	//! Sets the initial height and width to be a square that is 80% of the smallest dimension.
	void calculateInitialWindowDimensions();

	//! Creates and render the window.
	void init(const char* title, int xpos, int ypos, bool fullscreen);
	//! Handles 'r' for reset, 'p' for promotion, 'c' for calculating possible moves in 2 turns, '`' for random move.
	void handleKeyDown(SDL_KeyboardEvent& key);
	//! Does not have an implementation.
	void handleKeyUp(SDL_KeyboardEvent& key);
	//! Responsible for resizing of the window.
	void resizeWindow(int windowWidth, int windowHeight);

	static SDL_Renderer* renderer;
	static int screenHeight;
	static int screenWidth;

private:
	bool isRunning;
	SDL_Window* window;
	bool frozen;
	Game* game;

};