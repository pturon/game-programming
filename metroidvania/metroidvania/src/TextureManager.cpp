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

void TextureManager::drawText(const char* text, SDL_Rect destRect, TTF_Font* font, SDL_Color color) {	
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	destRect.w = surface->w;
	destRect.h = surface->h;
	SDL_Texture* message = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_RenderCopy(Game::renderer, message, NULL, &destRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(message);
}
