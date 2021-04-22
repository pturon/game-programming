#ifndef TextureManager_h
#define TextureManager_h
#include "Game.h"

class TextureManager {
public:
	static SDL_Texture* loadTexture(const char* filename, SDL_Renderer* r);
};

#endif
