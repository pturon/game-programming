#include "../include/TileComponent.h"
#include "../include/TextureManager.h"

TileComponent::TileComponent(int srcX, int srcY, int x, int y, const char* path) {

	texture = TextureManager::loadTexture(path);

	position.x = x;
	position.y = y; 

	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = TILE_WIDTH;
	srcRect.h = TILE_HEIGHT;

	destRect.x = x;
	destRect.y = y;
	destRect.w = TILE_WIDTH;
	destRect.h = TILE_HEIGHT;
}

void TileComponent::render() {
	TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
}

void TileComponent::update() {
	destRect.x = position.x - Game::camera.x;
	destRect.y = position.y - Game::camera.y;
}