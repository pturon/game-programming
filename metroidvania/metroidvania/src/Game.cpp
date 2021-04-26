#include "../include/Game.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent*>Game::colliders;

Manager manager; 
TileMap map;
SDL_Rect Game::camera = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

bool Game::isRunning = false; 

const char* tileSet = "assets/tileset.png";

auto& player(manager.addEntity());

auto& tiles(manager.getGroup(groupMap));

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

	map.loadMap("assets/levels/level_1.map");
	std::cout << map.width << std::endl; 
	std::cout << map.height << std::endl; 

	player.addComponent<TransformComponent>((WINDOW_WIDTH-PLAYER_WIDTH)/2, (WINDOW_HEIGHT-PLAYER_HEIGHT) / 2, PLAYER_WIDTH, PLAYER_HEIGHT, 1);
	player.addComponent<SpriteComponent>("assets/hero_spritesheet.png", true);
	player.addComponent<KeyboardController>();	
	player.addComponent<ColliderComponent>("Player");
	player.addGroup(groupPlayers);
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

	camera.x = player.getComponent<TransformComponent>().position.x - ((WINDOW_WIDTH-PLAYER_WIDTH) / 2);
	camera.y = player.getComponent<TransformComponent>().position.y - ((WINDOW_HEIGHT-PLAYER_HEIGHT) / 2);

	if (camera.x < 0) {
		camera.x = 0; 
	}
	if (camera.y < 0) {
		camera.y = 0;
	}
	if (camera.x > (map.width*TILE_WIDTH) - camera.w) {
		camera.x = (map.width * TILE_WIDTH) - camera.w;
	}
	if (camera.y > (map.height*TILE_HEIGHT) - camera.h) {
		camera.y = (map.height * TILE_HEIGHT) - camera.h;
	}

}

void Game::render() {
	SDL_RenderClear(renderer);
	manager.render();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::addTile(int srcX, int srcY, int x, int y) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, x, y, tileSet);
	tile.addComponent<ColliderComponent>("Wall");
	tile.addGroup(groupMap);
}