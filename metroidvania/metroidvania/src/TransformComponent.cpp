#include "../include/TransformComponent.h"

void TransformComponent::init() {
	velocity.zero();
	direction = right;
	state = &parent->getComponent<StateComponent>();
}

void TransformComponent::update() {
	if (velocity.y > 0) {
		velocity.y += fallMultiplier;
	}
	else if (velocity.y < 0 && !spaceDown) {
		velocity.y += lowJumpMultiplier;
	}
	if (state) {
		if (state->currentState == jumping) {
			float t = 0.16f;
			position.x += velocity.x * speed;
			position.y += t * velocity.y;
			velocity.x += t * (gravity.x * speed);
			velocity.y += t * gravity.y;
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
	direction = left; 
	velocity.x = -1; 
	parent->getComponent<SpriteComponent>().flipAnimation(true);
	if (state->currentState == idle || state->currentState == walking) {
		state->setState(walking);		
	}
}

void TransformComponent::moveRight() {
	direction = right;
	velocity.x = 1;
	parent->getComponent<SpriteComponent>().flipAnimation(false);
	if (state->currentState == idle || state->currentState == walking) {
		state->setState(walking);		
	}
}

void TransformComponent::moveStop() {
	velocity.x = 0; 
	if (state->currentState == idle || state->currentState == walking) {
		state->setState(idle);
	}
}

void TransformComponent::jump() {
	if (state->currentState != jumping && state->currentState != falling) {
		j = true; 
		state->setState(jumping);
		velocity.y = static_cast<float>(-jumpHeight);
	}
}

void TransformComponent::stopJump() {	
	if (state->currentState == jumping || state->currentState == falling) {
		if (velocity.x == 0) {
			state->setState(idle);
		}
		else {
			state->setState(walking);
		}
		velocity.y = 0;
	}	
}

void TransformComponent::startFall() {
	state->setState(falling);
	velocity.y = 0;
}