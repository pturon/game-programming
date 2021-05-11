#include "../include/TransformComponent.h"

void TransformComponent::init() {
	velocity.zero();
	direction = right;
	state = &parent->getComponent<StateComponent>();
}

void TransformComponent::update() {
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
			if (SDL_GetTicks() >= dashStart + dashDuration) {
				stopDash();
			}
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
}

void TransformComponent::moveLeft() {
	if (!state->isAttacking()) {
		direction = left;
		velocity.x = -1;
		parent->getComponent<SpriteComponent>().flipAnimation(true);
		if (state->currentState == idle || state->currentState == walking) {
			state->setState(walking);
		}
	}	
}

void TransformComponent::moveRight() {
	if (!state->isAttacking()) {
		direction = right;
		velocity.x = 1;
		parent->getComponent<SpriteComponent>().flipAnimation(false);
		if (state->currentState == idle || state->currentState == walking) {
			state->setState(walking);
		}
	}	
}

void TransformComponent::moveStop() {
	if (!state->isAttacking()) {
		velocity.x = 0;
		if (state->currentState == idle || state->currentState == walking) {
			state->setState(idle);
		}
	}	
}

void TransformComponent::jump() {
	if (!state->isAttacking()) {
		if (state->currentState != jumping && state->currentState != falling) {
			j = true;
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