#include "../include/Game.h"

Hero* hero;
TileMap* map;

SDL_Renderer* Game::renderer = nullptr;

Game::Game() {
	
}

Game::~Game() {

}

void Game::init(const char* title, int width, int height, bool fullscreen) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		int flags = 0; 
		if (fullscreen) {
			flags = SDL_WINDOW_FULLSCREEN;
		}
		else {
			flags = SDL_WINDOW_SHOWN;
		}

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width, height, flags);	
		renderer = SDL_CreateRenderer(window, -1, 0);		

		isRunning = true; 		
	
	}
	else {
		isRunning = false; 
	}

	hero = new Hero();
	map = new TileMap();
}

void Game::handleEvents() {
	SDL_Event event; 
	SDL_PollEvent(&event);
	switch (event.type)	{
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

void Game::update() {
	hero->update();
}

void Game::render() {
	SDL_RenderClear(renderer);
	map->render();
	hero->render();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}