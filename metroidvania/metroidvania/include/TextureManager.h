#pragma once
#include "Game.h"

class TextureManager {
public:
	static SDL_Texture* loadTexture(const char* filename);
	static void draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect);
};

