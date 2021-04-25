#include "../include/TileComponent.h"
#include "../include/TextureManager.h"

TileComponent::TileComponent(int srcX, int srcY, int x, int y, const char* path) {

	texture = TextureManager::loadTexture(path);

	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = 32;
	srcRect.h = 32;

	destRect.x = x;
	destRect.y = y;
	destRect.w = 32;
	destRect.h = 32;
}

void TileComponent::render() {
	TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
}