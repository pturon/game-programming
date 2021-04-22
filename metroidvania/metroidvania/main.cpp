#include "SDL.h"
#include "include/Game.h"

//Sceen dimension constants
const int WINDOW_WIDTH = 600; 
const int WINDOW_HEIGHT = 400; 

Game* game = nullptr;

int main(int argc, char* argv[]) {

	game = new Game(); 
	game->init("Metroidvania", WINDOW_WIDTH, WINDOW_HEIGHT, false);

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
	}
	game->clean();

	return 0;
}