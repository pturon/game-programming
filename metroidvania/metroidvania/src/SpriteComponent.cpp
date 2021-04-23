#include "../include/SpriteComponent.h"


SpriteComponent::SpriteComponent(const char *path) {
	texture = TextureManager::loadTexture(path);
}

void SpriteComponent::init(){
	srcRect.x = srcRect.y = 0;
	srcRect.w = srcRect.h = 100;
	destRect.w = destRect.h = 100;

	position = &parent->getComponent<PositionComponent>();
}

void SpriteComponent::update() {
	destRect.x = position->x();
	destRect.y = position->y();
}

void SpriteComponent::render() {
	std::cout << "Moin";
	TextureManager::draw(texture, srcRect, destRect);
}



