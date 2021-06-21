#include "../include/TransformComponent.h"

void TransformComponent::init() {
	velocity.zero();
	direction = right;
	state = &parent->getComponent<StateComponent>();
	lastPos = position;
}

void TransformComponent::update() {
	lastPos = position;
	if (velocity.y > 0) {
		velocity.y += fallMultiplier;
		if (state) {
			if (state->isAttacking()) {
				state->lastState = falling; 
			}
			else {
				state->setState(falling);
			}			
		}
	}
	else if (velocity.y < 0 && !spaceDown) {
		velocity.y += lowJumpMultiplier;
	}
	if (state) {
		if (state->currentState == jumping || state->currentState==falling || (state->isAttacking() && (state->lastState == falling || state->lastState == jumping))) {
			float t = 0.16f;
			position.x += velocity.x * speed;
			position.y += t * velocity.y;
			velocity.x += t * (gravity.x * speed);
			velocity.y += t * gravity.y;
		}
		else if (state->currentState == dashing) {
			position.x += velocity.x * dashSpeed;
			velocity.y = 0;
			if (SDL_GetTicks() >= dashStart + dashDuration) {
				state->setState(falling);
			}
			
		}
		else if (state->currentState == wallCling) {
			position.y += gravity.y / 3; 
		}
		else {
			position.x += velocity.x * speed;
			position.y += velocity.y * speed;
			if (gravityAffected) {
				position.x += gravity.x;
				position.y += gravity.y;
			}
		}
	}	
	if (recoil) {
		if (recoilDuration > 0) {
			position.x += recoilV.x * recoilSpeed;
			position.y += recoilV.y * recoilSpeed;
			recoilDuration--;
		}
		else {
			recoil = false; 
		}		
	}
}

void TransformComponent::moveLeft() {
	if (state->currentState != wallCling || (state->currentState == wallCling && clingedWallPos == right)) {
		if (!state->isAttacking() && state->currentState != dashing) {
			stopWallCling();
			direction = left;
			velocity.x = -1;
			parent->getComponent<SpriteComponent>().flipAnimation(true);
			if (state->currentState == idle || state->currentState == walking) {
				state->setState(walking);
			}
		}
	}	
}

void TransformComponent::moveRight() {
	if (state->currentState != wallCling || (state->currentState == wallCling && clingedWallPos == left)) {
		if (!state->isAttacking() && state->currentState != dashing) {
			stopWallCling();
			direction = right;
			velocity.x = 1;
			parent->getComponent<SpriteComponent>().flipAnimation(false);
			if (state->currentState == idle || state->currentState == walking) {
				state->setState(walking);
			}
		}
	}	
}

void TransformComponent::moveStop() {
	if (!state->isAttacking() && state->currentState != dashing) {
		velocity.x = 0;
		if (state->currentState == idle || state->currentState == walking) {
			state->setState(idle);
		}
	}	
}

void TransformComponent::jump() {
	if (!state->isAttacking()) {
		if (state->currentState != jumping && state->currentState != falling) {
			stopWallCling();
			state->setState(jumping);
			velocity.y = static_cast<float>(-jumpHeight);
		}
		else if (state->currentState == wallCling) {
			stopWallCling();
			state->setState(jumping);
			velocity.y = static_cast<float>(-jumpHeight);
		}
		else if (canDoubleJump) {
			canDoubleJump = false; 
			state->setState(jumping);
			velocity.y = static_cast<float>(-jumpHeight);
		}
	}	
}

void TransformComponent::stopJump() {	
	if (state->currentState == jumping || state->currentState == falling) {
		if (velocity.x == 0) {
			if (state->isAttacking()) {
				state->lastState = idle; 
			}
			else {
				state->setState(idle);
			}			
		}
		else {
			if (state->isAttacking()) {
				state->lastState = walking;
			}
			else {
				state->setState(walking);
			}			
		}
		velocity.y = 0;
	}	
	canDash = true; 
	canDoubleJump = true; 
}

void TransformComponent::startFall() {
	if (state->isAttacking()) {
		state->lastState = falling; 
	}
	else {
		state->setState(falling);
	}	
	velocity.y = 0;
}

void TransformComponent::dash() {
	if (state->currentState != dashing && canDash) {
		canDash = false; 
		velocity.y = 0;
		switch (direction){
		case left: 
			velocity.x = -1; 
			break; 
		case right:
			velocity.x = 1; 
			break; 
		default:
			break;
		}
		state->setState(dashing);
		dashStart = SDL_GetTicks();
	}	
}

void TransformComponent::stopDash() {
	state->setState(falling);
	canDash = true;
}

void TransformComponent::startWallCling(Direction d) {
	if (state->currentState != wallCling) {
		state->setState(wallCling);
		velocity.x = 0;
		velocity.y = 0;
		clingedWallPos = d;
	}	
}

void TransformComponent::stopWallCling() {
	if (state->currentState == wallCling) {
		state->setState(falling);
	}	
}

int TransformComponent::getYDistance() {
	return 9; 
}

void TransformComponent::startRecoil(Vector2D rV) {
	recoil = true; 
	recoilDuration = 10; 
	recoilV = rV; 
}