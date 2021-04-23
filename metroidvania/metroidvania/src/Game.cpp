#include "../include/Game.h"

TileMap* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager; 

auto& player(manager.addEntity());
auto& wall(manager.addEntity());

Game::Game() {
	isRunning = false;
	window = nullptr;
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
	map = new TileMap();

	player.addComponent<TransformComponent>(0.0f, 0.0f, 100, 100, 1);
	player.addComponent<SpriteComponent>("assets/hero.png");
	player.addComponent<KeyboardController>();	
	player.addComponent<ColliderComponent>("Player");

	wall.addComponent<TransformComponent>(300.0f, 0.0f, 50, 300, 1);
	wall.addComponent<SpriteComponent>("assets/dummy.png");
	wall.addComponent<ColliderComponent>("Wall");
}

void Game::handleEvents() {
	SDL_PollEvent(&Game::event);
	switch (event.type)	{
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

void Game::update() {
	manager.refresh();
	manager.update();	
	if (Collision::AABB(player.getComponent<ColliderComponent>().collider, wall.getComponent<ColliderComponent>().collider)) {
		player.getComponent<TransformComponent>().velocity * -1;
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	map->render();
	manager.render();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}