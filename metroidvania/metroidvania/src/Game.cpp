#include "../include/Game.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager; 
Map* m; 
SDL_Rect Game::camera = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
HudManager hudManager;
Transition transition;

bool Game::isRunning = false; 
bool pause = false; 

auto& player(manager.addEntity());
auto& colliders(manager.getGroup(groupColliders));
auto& transitions(manager.getGroup(groupTransitions));

Game::Game() {
	isRunning = false;
	window = nullptr;
}

Game::~Game() {

}

void Game::init(const char* title, int width, int height, bool fullscreen) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		TTF_Init();
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
	
	hudManager.init();

	//map = new TileMap("assets/tileSet.png");
	//map->loadMap("level_1");
	//map->loadColliders("level_1");

	m = new Map();
	m->tileSetPath = "assets/tileSet.png";
	m->loadMap("level_1");

	player.addComponent<StatsComponent>(5,100,0,3,1);
	player.addComponent<StateComponent>(); 
	player.addComponent<KeyboardController>();
	player.addComponent<TransformComponent>(64, 64, PLAYER_WIDTH, PLAYER_HEIGHT, 1, 5, true);
	player.addComponent<ColliderComponent>("Player");
	player.addComponent<SpriteComponent>("assets/hero_spritesheet.png", true);
	player.addComponent<AttackComponent>();
	player.getComponent<KeyboardController>().getComponents();
	player.addGroup(groupPlayers);

	hudManager.playerStats = &player.getComponent<StatsComponent>();
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
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_p) {
			pause = !pause; 
		}
		else if (event.key.keysym.sym == SDLK_m) {			
			player.getComponent<StatsComponent>().curMana -= 1; 
		}
		else if (event.key.keysym.sym == SDLK_n) {	
			player.getComponent<StatsComponent>().money += 1;
		}
		else if (event.key.keysym.sym == SDLK_b) {			
			player.getComponent<StatsComponent>().souls += 1;
		} 
		else if (event.key.keysym.sym == SDLK_v) {
			m->clearMap();
			m->loadMap("level_2");
		}
	}
}

void Game::update() {
	if (!pause) {
		player.getComponent<ColliderComponent>().update();
		SDL_Rect playerColliderPosBefore = player.getComponent<ColliderComponent>().collider;
		
		manager.refresh();
		manager.update();

		SDL_Rect playerColliderPos = player.getComponent<ColliderComponent>().collider;		

		Vector2D playerVelocity = { static_cast<float>(playerColliderPos.x - playerColliderPosBefore.x),  static_cast<float>(playerColliderPos.y - playerColliderPosBefore.y) };
		Vector2D cp, cn;
		float ct;		
		std::vector<std::pair<Entity*, float>> z; 

		for (auto& c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (Collision::DynamicRectRect(playerColliderPosBefore, playerVelocity, cCol, cp, cn, ct)) {					
				z.push_back({ c,ct });
			}			
		}

		std::sort(z.begin(), z.end(), [](const std::pair<Entity*, float>& a, const std::pair<Entity*, float>& b) {
			return a.second < b.second;
		});
		for (auto c : z) {
			SDL_Rect cCol = c.first->getComponent<ColliderComponent>().collider;
			if (Collision::DynamicRectRect(playerColliderPosBefore, playerVelocity, cCol, cp, cn, ct)) {				
				playerVelocity.x += cn.x * std::abs(playerVelocity.x) * (1 - ct);
				playerVelocity.y += cn.y * std::abs(playerVelocity.y) * (1 - ct);	
				if (cn.x == 0 && cn.y == -1) {
					player.getComponent<TransformComponent>().stopJump();
					if (player.getComponent<StateComponent>().currentState == wallCling) {
						player.getComponent<TransformComponent>().stopWallCling();
					}
				}
				else if (cn.x == 0 && cn.y == 1) {
					player.getComponent<TransformComponent>().startFall();
				}
				else if (cn.x == -1 && cn.y == 0) {
					if (player.getComponent<StateComponent>().currentState == dashing) {
						player.getComponent<TransformComponent>().stopDash();
					}
					if (player.getComponent<StateComponent>().currentState != walking) {
						player.getComponent<TransformComponent>().startWallCling(right);
					}
				}
				else if (cn.x == 1 && cn.y == 0) {
					if (player.getComponent<StateComponent>().currentState == dashing) {
						player.getComponent<TransformComponent>().stopDash();
					}
					if (player.getComponent<StateComponent>().currentState != walking) {
						player.getComponent<TransformComponent>().startWallCling(left);
					}
				}
			}
		}		

		

		player.getComponent<TransformComponent>().position.x = playerColliderPosBefore.x + playerVelocity.x;
		player.getComponent<TransformComponent>().position.y = playerColliderPosBefore.y + playerVelocity.y;
		player.getComponent<ColliderComponent>().update();
		SDL_Rect pCol = player.getComponent<ColliderComponent>().collider;

		if (player.getComponent<StateComponent>().currentState == wallCling) {
			bool stopWallCling = true;
			for (auto& c : colliders) {
				SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
				if (player.getComponent<TransformComponent>().clingedWallPos == right) {
					if (cCol.x == pCol.x + pCol.w && cCol.y + cCol.h >= pCol.y && cCol.y < pCol.y + pCol.h) {
						stopWallCling = false;
					}
				}
				else {
					if (cCol.x + cCol.w == pCol.x && cCol.y + cCol.h >= pCol.y && cCol.y < pCol.y + pCol.h) {
						stopWallCling = false;
					}
				}
			}

			if (stopWallCling) {
				player.getComponent<TransformComponent>().stopWallCling();
			}
		}		

		for (auto &t : transitions) {
			SDL_Rect tCol = t->getComponent<TransitionComponent>().collider;
			if (Collision::RectRect(tCol, pCol)) {
				transition.fadeIn();
				m->clearMap();
				m->loadMap(t->getComponent<TransitionComponent>().level);
				player.getComponent<TransformComponent>().position.x = t->getComponent<TransitionComponent>().newX;
				player.getComponent<TransformComponent>().position.y = t->getComponent<TransitionComponent>().newY;
				transition.fadeOut();
			}
		}

		camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - ((WINDOW_WIDTH - PLAYER_WIDTH) / 2));
		camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - ((WINDOW_HEIGHT - PLAYER_HEIGHT) / 2));

		if (camera.x < 0) {
			camera.x = 0;
		}
		if (camera.y < 0) {
			camera.y = 0;
		}
		if (camera.x > (m->width * TILE_WIDTH) - camera.w) {
			camera.x = (m->width * TILE_WIDTH) - camera.w;
		}
		if (camera.y > (m->height * TILE_HEIGHT) - camera.h) {
			camera.y = (m->height * TILE_HEIGHT) - camera.h;
		}
	}
}

void Game::render() {	
	SDL_RenderClear(renderer);
	manager.render();
	hudManager.render();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
