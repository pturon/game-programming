#pragma once
#include "EntityComponentSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"

class TileComponent; 
class SpriteComponent; 

class TileComponent : public Component {
public: 
	TransformComponent* transform; 
	SpriteComponent* sprite;

	SDL_Rect tileRect; 
	int tileID;

	TileComponent() = default; 
	TileComponent(int x, int y, int w, int h, int id) {
		tileRect.x = x; 
		tileRect.y = y; 
		tileRect.w = w; 
		tileRect.h = h; 
		tileID = id; 
	}

	void init() override {
		parent->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 1);
		transform = &parent->getComponent<TransformComponent>();
		parent->addComponent<SpriteComponent>("assets/dummy.png");
		sprite = &parent->getComponent<SpriteComponent>();
	}
};