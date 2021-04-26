#include "SDL.h"
#include "include/Game.h"
#include "include/Constants.h"

const int FPS = 60;
const int frameDelay = 1000 / FPS;

Game* game = nullptr;

int main(int argc, char* argv[]) {

	Uint32 frameStart;
	int frameTime; 

	game = new Game(); 
	game->init("Metroidvania", WINDOW_WIDTH, WINDOW_HEIGHT, false);

	while (game->running()) {

		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();

	return 0;
}