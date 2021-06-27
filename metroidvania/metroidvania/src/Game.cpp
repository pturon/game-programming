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
bool playerDeath = false; 

Entity& Game::player(manager.addEntity());
auto& enemy(manager.addEntity());
auto& enemy2(manager.addEntity());
auto& colliders(manager.getGroup(groupColliders));
auto& transitions(manager.getGroup(groupTransitions));
auto& enemies(manager.getGroup(groupEnemies));

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

	player.addComponent<StatsComponent>(5,100,0,3,1,60);
	player.addComponent<StateComponent>(); 
	player.addComponent<KeyboardController>();
	player.addComponent<TransformComponent>(64, 64, PLAYER_WIDTH, PLAYER_HEIGHT, 1, 5, true);
	player.addComponent<ColliderComponent>("Player", 30, 0);
	player.addComponent<SpriteComponent>("assets/hero_v2_spritesheet.png", true);
	player.addComponent<AttackComponent>();
	player.getComponent<KeyboardController>().getComponents();
	player.addGroup(groupPlayers);
	player.getComponent<SpriteComponent>().addAnimation(idle, 0, 1, 100);
	player.getComponent<SpriteComponent>().addAnimation(walking, 1, 6, 100);
	player.getComponent<SpriteComponent>().addAnimation(jumping, 2, 2, 100);
	player.getComponent<SpriteComponent>().addAnimation(falling, 3, 2, 100);
	player.getComponent<SpriteComponent>().addAnimation(attackingSide, 6, 5, 100);
	player.getComponent<SpriteComponent>().addAnimation(attackingTop, 7, 5, 100);
	player.getComponent<SpriteComponent>().addAnimation(attackingBottom, 8, 5, 100);
	player.getComponent<SpriteComponent>().addAnimation(dashing, 4, 2, 100);
	player.getComponent<SpriteComponent>().addAnimation(wallCling, 5, 1, 100);
	player.getComponent<SpriteComponent>().switchAnimation(idle);

	enemy.addComponent<StateComponent>();
	enemy.addComponent<TransformComponent>(200, 64, 48,16, 1,2, true);
	enemy.addComponent<ColliderComponent>("Enemy", 30, 0);
	enemy.addComponent<SpriteComponent>("assets/rat_spritesheet.png", true);
	enemy.addComponent<BehaviourComponent>();
	enemy.getComponent<BehaviourComponent>().setBehaviour<GoombaBehaviour>();
	enemy.addComponent<StatsComponent>(5,0,0,1,0, 60);
	enemy.addGroup(groupEnemies);
	enemy.getComponent<SpriteComponent>().addAnimation(idle, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(walking, 0, 4, 100);
	enemy.getComponent<SpriteComponent>().switchAnimation(walking);
	enemy.getComponent<SpriteComponent>().addAnimation(dying, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(dead, 0, 1, 100);

	enemy2.addComponent<StateComponent>();
	enemy2.addComponent<TransformComponent>(608, 400, 64, 64, 1, 2, true);
	enemy2.addComponent<ColliderComponent>("Enemy", 30, 0);
	enemy2.addComponent<SpriteComponent>("assets/skeleton_spritesheet.png", true);
	enemy2.addComponent<BehaviourComponent>();
	enemy2.getComponent<BehaviourComponent>().setBehaviour<SkeletonBehaviour>();
	enemy2.addComponent<StatsComponent>(5, 0, 0, 1, 0, 60);
	enemy2.addComponent<AttackComponent>();
	enemy2.addGroup(groupEnemies);
	enemy2.getComponent<SpriteComponent>().addAnimation(idle, 0, 1, 100);
	enemy2.getComponent<SpriteComponent>().addAnimation(walking, 0, 1, 100);
	enemy2.getComponent<SpriteComponent>().addAnimation(attackingSide, 0, 1, 100);
	enemy2.getComponent<SpriteComponent>().addAnimation(attackCooldown, 0, 1, 100);
	enemy2.getComponent<SpriteComponent>().addAnimation(charge, 0, 1, 100);
	enemy2.getComponent<SpriteComponent>().addAnimation(dying, 0, 1, 100);
	enemy2.getComponent<SpriteComponent>().addAnimation(dead, 0, 1, 100);
	enemy2.getComponent<StateComponent>().setState(walking);

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
}

void Game::update() {

	/**
	bool step = true; 
	while (step) {
		SDL_PollEvent(&Game::event);
		if (Game::event.type == SDL_KEYDOWN) {
			if (Game::event.key.keysym.sym == SDLK_u) {
				step = false;
			}
		}
	}	
	**/

	if (!pause) {
		
		SDL_Rect playerColliderPosBefore;
		if (!playerDeath) {
			
			player.getComponent<ColliderComponent>().update();
			playerColliderPosBefore = player.getComponent<ColliderComponent>().collider;
		}

		manager.refresh();
		
		manager.update();

		if (!playerDeath) {
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
			std::cout << playerColliderPosBefore.x + playerVelocity.x << std::endl;
			player.getComponent<TransformComponent>().position.x = playerColliderPosBefore.x + playerVelocity.x - player.getComponent<ColliderComponent>().xOffset;
			player.getComponent<TransformComponent>().position.y = playerColliderPosBefore.y + playerVelocity.y - player.getComponent<ColliderComponent>().yOffset;
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

			for (auto& t : transitions) {
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

			for (auto& e : enemies) {
				SDL_Rect eBefore = { e->getComponent<TransformComponent>().lastPos.x, e->getComponent<TransformComponent>().lastPos.y, e->getComponent<TransformComponent>().width, e->getComponent<TransformComponent>().height };
				Vector2D eVelocity = { e->getComponent<TransformComponent>().position.x - e->getComponent<TransformComponent>().lastPos.x, e->getComponent<TransformComponent>().position.y - e->getComponent<TransformComponent>().lastPos.y };

				std::vector<std::pair<Entity*, float>> eZ;

				for (auto& c : colliders) {
					SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
					if (Collision::DynamicRectRect(eBefore, eVelocity, cCol, cp, cn, ct)) {
						eZ.push_back({ c,ct });
					}
				}

				std::sort(eZ.begin(), eZ.end(), [](const std::pair<Entity*, float>& a, const std::pair<Entity*, float>& b) {
					return a.second < b.second;
					});
				for (auto c : eZ) {
					SDL_Rect cCol = c.first->getComponent<ColliderComponent>().collider;
					if (Collision::DynamicRectRect(eBefore, eVelocity, cCol, cp, cn, ct)) {
						eVelocity.x += cn.x * std::abs(eVelocity.x) * (1 - ct);
						eVelocity.y += cn.y * std::abs(eVelocity.y) * (1 - ct);
						e->getComponent<BehaviourComponent>().onCollision(cn);
					}
				}
				e->getComponent<TransformComponent>().position.x = eBefore.x + eVelocity.x;
				e->getComponent<TransformComponent>().position.y = eBefore.y + eVelocity.y;
				e->getComponent<ColliderComponent>().update();
			}

			for (auto& e : enemies) {		
				if (e->getComponent<StateComponent>().currentState != dying && e->getComponent<StateComponent>().currentState != dead) {
					if (player.getComponent<StateComponent>().isAttacking()) {							
						if (Collision::RectRect(player.getComponent<AttackComponent>().attackCollider, e->getComponent<ColliderComponent>().collider)) {
						
							if (e->getComponent<StatsComponent>().iFrames == 0) {
								e->getComponent<StatsComponent>().curHealth -= player.getComponent<StatsComponent>().attackDamage;
								e->getComponent<StatsComponent>().iFrames = e->getComponent<StatsComponent>().maxIFrames;
								e->getComponent<BehaviourComponent>().onHit(player.getComponent<TransformComponent>().direction);
								if (player.getComponent<TransformComponent>().direction == down) {
									player.getComponent<TransformComponent>().pogo();
								}
							}
						}
					}
					if (e->hasComponent<AttackComponent>()) {
						if (e->getComponent<StateComponent>().isAttacking()) {
							SDL_Rect eAttack = { e->getComponent<AttackComponent>().attackCollider.x + camera.x, e->getComponent<AttackComponent>().attackCollider.y + camera.y, e->getComponent<AttackComponent>().attackCollider.w, e->getComponent<AttackComponent>().attackCollider.h };
							if (Collision::RectRect(player.getComponent<ColliderComponent>().collider, eAttack)) {								
								if (player.getComponent<StatsComponent>().iFrames == 0) {
									player.getComponent<StatsComponent>().curHealth -= e->getComponent<StatsComponent>().attackDamage;
									player.getComponent<StatsComponent>().iFrames = player.getComponent<StatsComponent>().maxIFrames;
								}
							}
						}
					}
					else {
						if (Collision::RectRect(player.getComponent<ColliderComponent>().collider, e->getComponent<ColliderComponent>().collider)) {
							if (player.getComponent<StatsComponent>().iFrames == 0) {
								player.getComponent<StatsComponent>().curHealth -= e->getComponent<StatsComponent>().attackDamage;
								player.getComponent<StatsComponent>().iFrames = player.getComponent<StatsComponent>().maxIFrames;
							}
						}
					}					
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

			for (auto& e : enemies) {
				if (e->getComponent<StatsComponent>().curHealth <= 0) {
					e->getComponent<BehaviourComponent>().die();
				}
			}
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

bool Game::rayHitsCollider(Vector2D origin, Vector2D ray) {
	bool hit = false;
	Vector2D cp, cn;
	float t;
	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::RayRect(origin, ray, cCol, cp, cn, t)) {
			if (t <= 1) {
				hit = true;
			}		
		}
	}
	return hit; 
}
