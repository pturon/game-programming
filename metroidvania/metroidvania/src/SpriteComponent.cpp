#include "../include/SpriteComponent.h"


SpriteComponent::SpriteComponent(const char *path) {
	texture = TextureManager::loadTexture(path);
}

void SpriteComponent::init(){
	srcRect.x = srcRect.y = 0;
	srcRect.w = srcRect.h = 100;
	destRect.w = destRect.h = 100;

	transform = &parent->getComponent<TransformComponent>();
}

void SpriteComponent::update() {
	destRect.x = (int)transform->position.x;
	destRect.y = (int)transform->position.y;
}

void SpriteComponent::render() {
	TextureManager::draw(texture, srcRect, destRect);
}



