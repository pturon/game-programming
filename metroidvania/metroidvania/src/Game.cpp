#include "../include/Game.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager; 
TileMap* map;
SDL_Rect Game::camera = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

bool Game::isRunning = false; 

auto& player(manager.addEntity());
auto& tiles(manager.getGroup(groupMap));
auto& colliders(manager.getGroup(groupColliders));

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

	map = new TileMap("assets/tileSet.png");
	map->loadMap("assets/levels/level_1.map");
	map->loadColliders("assets/levels/level_1_colliders.map");

	player.addComponent<StateComponent>(); 
	player.addComponent<KeyboardController>();
	player.addComponent<TransformComponent>(864, 256, PLAYER_WIDTH, PLAYER_HEIGHT, 1, 5, true);
	player.addComponent<ColliderComponent>("Player");
	player.addComponent<SpriteComponent>("assets/hero_spritesheet.png", true);
	player.addComponent<AttackComponent>();
	player.getComponent<KeyboardController>().getComponents();
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
	SDL_Rect playerBefore = player.getComponent<ColliderComponent>().collider;
	manager.refresh();
	manager.update();		
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;

	int x = playerCol.x - playerBefore.x;
	int y = playerCol.y - playerBefore.y;
	
	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {			
			
			//Player is above
			if (playerCol.y + playerCol.h > cCol.y && playerCol.y < cCol.y) {
				player.getComponent<TransformComponent>().stopJump();
			}
			//Player is below
			else if (playerCol.y < cCol.y + cCol.h && playerCol.y + playerCol.h > cCol.y + cCol.h) {
				player.getComponent<TransformComponent>().startFall();
			}
			if (cCol.y < playerCol.y + playerCol.h && cCol.y + cCol.h > playerCol.y) {
				//Player is right
				if (playerCol.x + playerCol.w > cCol.x && playerCol.x < cCol.x) {					
					if (player.getComponent<StateComponent>().currentState == dashing) {
						player.getComponent<TransformComponent>().stopDash();
					}
					if (player.getComponent<StateComponent>().currentState != walking) {
						player.getComponent<TransformComponent>().startWallCling(right);
					}					
				}
				//Player is left
				else if (playerCol.x < cCol.x + cCol.w && playerCol.x + playerCol.h > cCol.x + cCol.w) {					
					if (player.getComponent<StateComponent>().currentState == dashing) {
						player.getComponent<TransformComponent>().stopDash();
					}
					if (player.getComponent<StateComponent>().currentState != walking) {
						player.getComponent<TransformComponent>().startWallCling(left);
					}				
				}
			}					
		}
	}

	player.getComponent<TransformComponent>().position.x = playerCol.x;
	player.getComponent<TransformComponent>().position.y = playerCol.y;
	player.getComponent<ColliderComponent>().collider.x = playerCol.x;
	player.getComponent<ColliderComponent>().collider.y = playerCol.y;

	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - ((WINDOW_WIDTH - PLAYER_WIDTH) / 2));
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - ((WINDOW_HEIGHT - PLAYER_HEIGHT) / 2));

	if (camera.x < 0) {
		camera.x = 0; 
	}
	if (camera.y < 0) {
		camera.y = 0;
	}
	if (camera.x > (map->width*TILE_WIDTH) - camera.w) {
		camera.x = (map->width * TILE_WIDTH) - camera.w;
	}
	if (camera.y > (map->height*TILE_HEIGHT) - camera.h) {
		camera.y = (map->height * TILE_HEIGHT) - camera.h;
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
