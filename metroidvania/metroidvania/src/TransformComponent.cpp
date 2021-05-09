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
	if (jumping) {
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

void TransformComponent::moveLeft() {
	direction = left; 
	velocity.x = -1; 
	if (state->currentState == idle || state->currentState == walking) {
		state->setState(walking);
		parent->getComponent<SpriteComponent>().flipAnimation(true);
	}
}

void TransformComponent::moveRight() {
	direction = right;
	velocity.x = 1;
	if (state->currentState == idle || state->currentState == walking) {
		state->setState(walking);
		parent->getComponent<SpriteComponent>().flipAnimation(false);
	}
}

void TransformComponent::moveStop() {
	velocity.x = 0; 
	if (state->currentState == idle || state->currentState == walking) {
		state->setState(idle);
	}
}