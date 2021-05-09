#include "../include/AttackComponent.h"
#include "../include/TextureManager.h"

void AttackComponent::init() {
	transform = &parent->getComponent<TransformComponent>();
	attackCollider = {0,0,0,0};
	texture = TextureManager::loadTexture("assets/dummy.png");
}

void AttackComponent::update() {
	Direction curDirection = transform->direction;
	switch (curDirection) {
	case up:
		attackCollider.x = transform->position.x - Game::camera.x;
		attackCollider.y = transform->position.y - ATTACK_RANGE - Game::camera.y;
		attackCollider.w = transform->width;
		attackCollider.h = ATTACK_RANGE;
		break;
	case right:
		attackCollider.x = transform->position.x + transform->width - Game::camera.x;
		attackCollider.y = transform->position.y - Game::camera.y;
		attackCollider.w = ATTACK_RANGE;
		attackCollider.h = transform->height;
		break;
	case down:
		attackCollider.x = transform->position.x - Game::camera.x;
		attackCollider.y = transform->position.y + transform->height - Game::camera.y;
		attackCollider.w = transform->width;
		attackCollider.h = ATTACK_RANGE;
		break;
	case left:
		attackCollider.x = transform->position.x - ATTACK_RANGE - Game::camera.x;
		attackCollider.y = transform->position.y - Game::camera.y;
		attackCollider.w = ATTACK_RANGE;
		attackCollider.h = transform->height;
		break;
	default:
		break;
	}
	if (SDL_GetTicks() - attackStart >= attackDuration) {
		attacking = false;
	}
}

void AttackComponent::render() {
	if (attacking) {
		SDL_Rect srcRect = { 0,0,32,32 };
		TextureManager::draw(texture, srcRect, attackCollider, SDL_FLIP_NONE);		
	}
}

void AttackComponent::attack() {
	if (!attacking) {
		attacking = true;
		attackStart = SDL_GetTicks();
		Direction curDirection = transform->direction;		
	}
}
	