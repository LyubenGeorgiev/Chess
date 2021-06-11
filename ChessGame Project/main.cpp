#include "Window.h"

#include <stdlib.h>
#include <ctime>

int main(int argc, char* args[])
{
	srand(time(NULL));
	
	Window* gameWindow = new Window;

	gameWindow->init("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false);
	gameWindow->run();

	delete gameWindow;

	return 0;
}