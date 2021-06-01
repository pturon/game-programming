#pragma once
#include "Game.h"
#include "SDL_ttf.h"

class TextureManager {
public:
	static SDL_Texture* loadTexture(const char* filename);
	static void draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip);
	static void drawText(const char* text, SDL_Rect destRect);
};

