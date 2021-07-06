#include "../include/AttackComponent.h"
#include "../include/TextureManager.h"

AttackComponent::AttackComponent(int aR) {
	attackRange = aR;
}

void AttackComponent::init() {
	transform = &parent->getComponent<TransformComponent>();
	attackCollider = {0,0,0,0};
	texture = TextureManager::loadTexture("assets/dummy.png");
	state = &parent->getComponent<StateComponent>();
}

void AttackComponent::update() {
	Direction curDirection = transform->direction;
	switch (curDirection) {
	case up:
		attackCollider.x = transform->position.x;
		attackCollider.y = transform->position.y - attackRange;
		attackCollider.w = transform->width;
		attackCollider.h = attackRange;
		break;
	case right:
		attackCollider.x = transform->position.x + transform->width - attackRange;
		attackCollider.y = transform->position.y;
		attackCollider.w = attackRange;
		attackCollider.h = transform->height;
		break;
	case down:
		attackCollider.x = transform->position.x;
		attackCollider.y = transform->position.y + transform->height;
		attackCollider.w = transform->width;
		attackCollider.h = attackRange;
		break;
	case left:
		attackCollider.x = transform->position.x;
		attackCollider.y = transform->position.y;
		attackCollider.w = attackRange;
		attackCollider.h = transform->height;
		break;
	default:
		break;
	}
	if (!parent->hasComponent<BehaviourComponent>()) {
		if (SDL_GetTicks() - attackStart >= attackDuration) {
			if (state->isAttacking()) {
				state->switchToLastState();
			}
		}
	}	

	destRect.x = attackCollider.x - Game::camera.x;
	destRect.y = attackCollider.y - Game::camera.y;
	destRect.w = attackCollider.w;
	destRect.h = attackCollider.h; 
}

void AttackComponent::render() {
	if (state->isAttacking()) {
		SDL_Rect srcRect = { 0,0,32,32 };
		TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);		
	}
}

void AttackComponent::attack() {
	if (!state->isAttacking() && state->currentState!=dashing) {
		attackStart = SDL_GetTicks();
		Direction curDirection = transform->direction;		
		switch (curDirection) {
		case up:
			state->setState(attackingTop);
			break;
		case left:
			state->setState(attackingSide);
			break;
		case right:
			state->setState(attackingSide);
			break;
		case down:
			state->setState(attackingBottom);			
			break;

		}
	}
}
	