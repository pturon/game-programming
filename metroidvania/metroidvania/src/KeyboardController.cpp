#include "../include/KeyboardController.h"

void KeyboardController::init() {
	
}

void KeyboardController::getComponents() {
	transform = &parent->getComponent<TransformComponent>();
	sprite = &parent->getComponent<SpriteComponent>();
}

void KeyboardController::update() {
	if (Game::event.type == SDL_KEYDOWN) {
		switch (Game::event.key.keysym.sym) {
		case SDLK_w:
			transform->direction = up; 
			break;
		case SDLK_a:
			leftDown = true;
			break;
		case SDLK_s:
			transform->direction = down;
			break;
		case SDLK_d:
			rightDown = true;
			break;
		case SDLK_SPACE:
			transform->jump();
			transform->spaceDown = true; 
			break;
		case SDLK_j:
			parent->getComponent<AttackComponent>().attack();
		default:
			break;
		}
	}
	if (Game::event.type == SDL_KEYUP) {
		switch (Game::event.key.keysym.sym) {
		case SDLK_w:

			break;
		case SDLK_a:
			leftDown = false;
			break;
		case SDLK_s:

			break;
		case SDLK_d:
			rightDown = false; 
			break;
		case SDLK_ESCAPE:
			Game::isRunning = false; 
			break; 
		case SDLK_SPACE: 
			transform->spaceDown = false;
			break; 
		default:
			break;
		}
	}

	if (leftDown && !rightDown) {
		transform->direction = left;
		sprite->switchAnimation("Walking");
		sprite->flipAnimation(true);
		transform->velocity.x = -1;
	}
	else if (!leftDown && rightDown) {
		transform->direction = right;
		sprite->switchAnimation("Walking");
		sprite->flipAnimation(false);
		transform->velocity.x = 1;
	}
	else {
		transform->velocity.x = 0;
		sprite->switchAnimation("Idle");
	}
	
}