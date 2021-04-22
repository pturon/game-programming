#include "../include/TextureManager.h"

SDL_Texture* TextureManager::loadTexture(const char* filename, SDL_Renderer* r) {
	SDL_Surface* tmpSurface = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(r, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return texture;
}
