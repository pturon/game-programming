#pragma once
#include "EntityComponentSystem.h"
#include "SDL.h"

class TileComponent; 
class SpriteComponent; 

class TileComponent : public Component {
public: 

	SDL_Texture* texture; 
	SDL_Rect srcRect, destRect; 

	TileComponent() = default; 
	~TileComponent() {
		SDL_DestroyTexture(texture);
	}
	TileComponent(int srcX, int srcY, int x, int y, const char* path);
	void render() override;
};