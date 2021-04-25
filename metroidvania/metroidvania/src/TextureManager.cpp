#include "../include/TextureManager.h"

SDL_Texture* TextureManager::loadTexture(const char* filename) {
	SDL_Surface* tmpSurface = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return texture;
}

void TextureManager::draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(Game::renderer, texture, &srcRect, &destRect, NULL, NULL, flip);
}
