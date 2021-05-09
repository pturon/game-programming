#include "../include/SpriteComponent.h"

//Animation mit Sate verknüpfen!
//Animationen per Methode hinzufügen!

SpriteComponent::SpriteComponent(const char *path) {
	texture = TextureManager::loadTexture(path);
}

SpriteComponent::SpriteComponent(const char* path, bool isAnimated) {
	animated = isAnimated; 

	Animation idle = Animation(0, 2, 100);
	Animation walking = Animation(1, 4, 100);
	Animation attack_side = Animation(2, 2, 100);
	Animation attack_top = Animation(3, 2, 100);
	Animation attack_bottom = Animation(4, 2, 100);
	Animation jumping = Animation(5, 1, 100);
	Animation falling = Animation(6, 1, 100);

	animations.emplace("Idle", idle);
	animations.emplace("Walking", walking);
	animations.emplace("Attack_Side", attack_side);
	animations.emplace("Attack_Top", attack_top);
	animations.emplace("Attack_Bottom", attack_bottom);
	animations.emplace("Jumping", jumping);
	animations.emplace("Falling", falling);

	switchAnimation("Idle");

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
	std::cout << animationIndex << std::endl; 
	destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	destRect.w = transform->width * transform->scale;
	destRect.h = transform->height * transform->scale;
	TextureManager::draw(texture, srcRect, destRect, spriteFlip);
}

void SpriteComponent::switchAnimation(const char* animationName) {
	frames = animations[animationName].frames; 
	animationIndex = animations[animationName].index;
	speed = animations[animationName].speed;
}

void SpriteComponent::flipAnimation(bool f) {
	if (f) {
		spriteFlip = SDL_FLIP_HORIZONTAL;
	}
	else {
		spriteFlip = SDL_FLIP_NONE;
	}
}



