#include "../include/SpriteComponent.h"

//Animation mit Sate verknüpfen!
//Animationen per Methode hinzufügen!

SpriteComponent::SpriteComponent(const char *path) {
	texture = TextureManager::loadTexture(path);
}

SpriteComponent::SpriteComponent(const char* path, bool isAnimated) {
	animated = isAnimated; 
	texture = TextureManager::loadTexture(path);
}

SpriteComponent::~SpriteComponent() {
	SDL_DestroyTexture(texture);
}

void SpriteComponent::init(){
	transform = &parent->getComponent<TransformComponent>();
	srcRect.x = srcRect.y = 0;
	srcRect.w = transform->width;
	srcRect.h = transform->height;	

	addAnimation(idle, 0, 2, 100);
	addAnimation(walking, 1, 4, 100);
	addAnimation(jumping, 5, 1, 100);
	addAnimation(falling, 6, 1, 100);
	addAnimation(attackingSide, 2, 2, 100);
	addAnimation(attackingTop, 3, 2, 100);
	addAnimation(attackingBottom, 4, 2, 100);
	switchAnimation(idle);
}

void SpriteComponent::update() {

	if (animated) {
		srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		srcRect.y = srcRect.h * animationIndex;
	}

	srcRect.y = animationIndex * transform->height;

	destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	destRect.w = transform->width * transform->scale;
	destRect.h = transform->height * transform->scale;
}

void SpriteComponent::render() {
	destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	destRect.w = transform->width * transform->scale;
	destRect.h = transform->height * transform->scale;
	TextureManager::draw(texture, srcRect, destRect, spriteFlip);
}

void SpriteComponent::switchAnimation(const State s) {
	frames = animations[s].frames; 
	animationIndex = animations[s].index;
	speed = animations[s].speed;
}

void SpriteComponent::flipAnimation(bool f) {
	if (f) {
		spriteFlip = SDL_FLIP_HORIZONTAL;
	}
	else {
		spriteFlip = SDL_FLIP_NONE;
	}
}

void SpriteComponent::addAnimation(State s, int index, int length, int frameDuration) {
	Animation a =  Animation(index, length, frameDuration);
	animations.emplace(s, a);
}



