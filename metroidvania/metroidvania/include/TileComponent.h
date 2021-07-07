#pragma once
#include "EntityComponentSystem.h"
#include "SDL.h"
#include "Constants.h"
#include "Vector2D.h"

class TileComponent; 
class SpriteComponent; 

class TileComponent : public Component {
public: 

	SDL_Texture* texture; 
	SDL_Rect srcRect, destRect; 
	Vector2D position;

	TileComponent() = default; 
	~TileComponent() {
		SDL_DestroyTexture(texture);
	}
	TileComponent(int srcX, int srcY, int x, int y, const char* path);
	void render() override;
	void update() override; 
};