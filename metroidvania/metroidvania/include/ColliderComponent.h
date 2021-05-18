#pragma once
#include "Components.h"
#include <string>
#include "SDL.h"
#include "TextureManager.h"
#include "Constants.h"

class TransformComponent; 

class ColliderComponent : public Component{
private:

public: 
	bool collided = false;
	SDL_Rect collider;
	std::string tag;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	TransformComponent* transform;

	ColliderComponent(std::string t) {
		tag = t;
	}
	ColliderComponent(std::string t, int x, int y, int w, int h) {
		tag = t;
		collider.x = x; 
		collider.y = y; 
		collider.w = w; 
		collider.h = h; 
	}

	void init() override;
	void update() override;
	void render() override;
};

