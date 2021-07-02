#include "../include/BossBehaviour.h"

void BossBehaviour::init() {
	state = &parent->getComponent<StateComponent>();
	stats = &parent->getComponent<StatsComponent>();
	transform = &parent->getComponent<TransformComponent>();
	transform->direction = left;
}

void BossBehaviour::update() {	
	if (transform->direction == right) {
		parent->getComponent<SpriteComponent>().flipAnimation(true);
	}
	else {
		parent->getComponent<SpriteComponent>().flipAnimation(false);
	}
	if (phase == 1 && stats->curHealth <= stats->maxHealth / 3 * 2) {			
		phase++;
		startRageJump();
	} else if (phase == 2 && stats->curHealth <= stats->maxHealth / 3) {		
		phase++;
		startRageJump();	
	}
	if (state->currentState == idle) {
		if (nextState == rageJump) {		
			startRageJump();
		}
		else if (nextState == idle) {
			Vector2D pos = { (parent->getComponent<TransformComponent>().position.x + parent->getComponent<TransformComponent>().width) / 2,  (parent->getComponent<TransformComponent>().position.y + parent->getComponent<TransformComponent>().height) / 2 };
			Vector2D playerPos = { (Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width) / 2,  (Game::player.getComponent<TransformComponent>().position.y + Game::player.getComponent<TransformComponent>().height) / 2 };;
			Vector2D dist = { playerPos.x - pos.x, playerPos.y - pos.y };
			int d = sqrt(dist.x * dist.x + dist.y * dist.y);
			if (d >= bludgeonRange || d > slamRange + leapRange) {
				startCharge();
				if (target.x == transform->position.x) {
					startLeapingBludgeon();
				}
			} 
			else {
				if (d == slamRange) {
					startSlam();
				}
				else if (d < slamRange) {
					startLeap();
				}
				else {
					int r = std::rand() % 2;
					if (r == 0) {
						startCharge();
						if (target.x == transform->position.x) {
							startLeapingBludgeon();
						}
					}
					else {
						startLeap();
					}
				}				
			}
			
		}		
	}
	else if (state->currentState == rage) {
		if (SDL_GetTicks() - tickStart >= rageAttackDuration) {
			rageAttackCount++;			
			tickStart = SDL_GetTicks();
			if (transform->direction == right) {
				transform->direction = left;
			}
			else if (transform->direction == left) {
				transform->direction = right;
			}
			if (rageAttackCount == rageAttackMax) {
				tickStart = SDL_GetTicks();
				state->setState(cooldown);				
				if (Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width / 2 >= transform->position.x + transform->width / 2) {
					transform->direction = right;
				}
				else {
					transform->direction = left;
				}
			}
		}
	}
	else if (state->currentState == charge) {
		if (transform->direction == right) {
			if (transform->position.x >= target.x) {
				transform->velocity.x = 0; 
				startLeapingBludgeon();
			}
		}
		else {
			if (transform->position.x <= target.x) {
				transform->velocity.x = 0;
				startLeapingBludgeon();
			}
		}
	}
	else if (state->currentState == cooldown) {
		if (SDL_GetTicks() - tickStart >= cooldownDuration) {
			state->setState(idle);		
			if (Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width / 2 >= transform->position.x + transform->width / 2) {
				transform->direction = right;
			}
			else {
				transform->direction = left;
			}
		}
	}
	else if (state->currentState == slam) {		
		if (SDL_GetTicks() - tickStart >= slamDuration) {
			tickStart = SDL_GetTicks();
			state->setState(cooldown);	
			if (Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width / 2 >= transform->position.x + transform->width / 2) {
				transform->direction = right;
			}
			else {
				transform->direction = left; 
			}
		}
	}
}

void BossBehaviour::onCollision(Vector2D cn) {
	if (state->currentState == rageJump) {
		if (cn.x == 0 && cn.y == -1) {
			transform->velocity.x = 0;
			transform->velocity.y = 0;
			startRage();
		}
	}
	else if (state->currentState == leapingBludgeon) {
		if (cn.x == 0 && cn.y == -1) {
			tickStart = SDL_GetTicks();
			transform->velocity.x = 0; 
			transform->velocity.y = 0; 
			state->setState(cooldown);	
			if (Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width / 2 >= transform->position.x + transform->width / 2) {
				transform->direction = right;
			}
			else {
				transform->direction = left;
			}
		}
	}
	else if (state->currentState == charge) {
		if (cn.x != 0 && cn.y == 0) {
			transform->velocity.x = 0;
			startLeapingBludgeon();
		}
	}
	else if (state->currentState == leaping) {
		transform->velocity.x = 0;
		transform->velocity.y = 0; 
		startSlam();
	}
}

void BossBehaviour::onHit(Direction d) {

}

void BossBehaviour::die() {

}

void BossBehaviour::startRage() {
	transform->velocity.x = 0; 
	transform->velocity.y = 0; 
	transform->direction = right;
	tickStart = SDL_GetTicks();
	rageAttackCount = 0;
	state->setState(rage);
}

void BossBehaviour::startRageJump() {	
	if (state->currentState != idle) {
		nextState = rageJump; 
	}
	else {	
		nextState = idle; 
		int jumpHeight = 80; 

	    int t = 69; 		
		float s = ((WINDOW_WIDTH - transform->width) / 2 - transform->position.x) / t / transform->speed;
		
		transform->velocity.x = s; 
		transform->velocity.y = static_cast<float>(-jumpHeight);
		state->setState(rageJump);
	}
}

void BossBehaviour::startLeap() {	
	Vector2D pos = { (parent->getComponent<TransformComponent>().position.x + parent->getComponent<TransformComponent>().width) / 2,  (parent->getComponent<TransformComponent>().position.y + parent->getComponent<TransformComponent>().height) / 2 };
	Vector2D playerPos = { (Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width) / 2,  (Game::player.getComponent<TransformComponent>().position.y + Game::player.getComponent<TransformComponent>().height) / 2 };;
	Vector2D dist = { playerPos.x - pos.x, playerPos.y - pos.y };
	int d = sqrt(dist.x * dist.x + dist.y * dist.y);
	if (dist.x >= 0) {
		transform->direction = right; 
		if (d >= slamRange) {
			transform->velocity.x = 0.5;
		}
		else {
			transform->velocity.x = -0.5;
		}
	}
	else {	
		transform->direction = left;
		if (d >= slamRange) {
			transform->velocity.x = -0.5;
		}
		else {
			transform->velocity.x = 0.5;
		}
	}	
	leapCounter++; 
	transform->velocity.y = static_cast<float>(-40);
	state->setState(leaping);
}

void BossBehaviour::startSlam() {
	leapCounter == 0; 
	state->setState(slam);
	tickStart = SDL_GetTicks();
}

void BossBehaviour::startLeapingBludgeon() {		
	state->setState(leapingBludgeon);
	leapCounter = 0; 
	int t = 69;
	float s = ((Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width/2) - transform->position.x) / t / transform->speed;
	transform->velocity.x = s; 
	transform->velocity.y = static_cast<float>(-80);
	if ((Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width / 2) >= transform->position.x + transform->width / 2) {
		transform->direction = right;
	}
	else {
		transform->direction = left;
	}
}

void BossBehaviour::startCharge() {	
	state->setState(charge);
	if (Game::player.getComponent<TransformComponent>().position.x >= transform->position.x) {
		target = { (Game::player.getComponent<TransformComponent>().position.x - bludgeonRange) - (transform->width / 2), transform->position.y };
	}
	else {
		target = { (Game::player.getComponent<TransformComponent>().position.x + bludgeonRange) - (transform->width / 2), transform->position.y };
	}
	if (target.x >= transform->position.x) {
		transform->velocity.x = 1; 
	}
	else {
		transform->velocity.x = -1;
	}	
}