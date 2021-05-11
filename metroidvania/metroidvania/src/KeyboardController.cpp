#include "../include/KeyboardController.h"

void KeyboardController::init() {
	
}

void KeyboardController::getComponents() {
	transform = &parent->getComponent<TransformComponent>();
	sprite = &parent->getComponent<SpriteComponent>();
	state = &parent->getComponent<StateComponent>();
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
			break; 
		case SDLK_k:
			transform->dash();
			break;
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
		transform->moveLeft();
	}
	else if (!leftDown && rightDown) {
		transform->moveRight();
	}
	else {
		transform->moveStop();
	}
	
}