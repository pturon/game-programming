#pragma once

#include "EntityComponentSystem.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "SDL.h"
#include "Enums.h"

class TransformComponent;
class AttackComponent; 

class AttackComponent : public Component {
private: 	
	int attackDuration = 200; 
	int attackStart = 0; 
	int lastTick = 0; 
	SDL_Texture* texture; 
public: 
	bool attacking = false;
	SDL_Rect attackCollider; 
	TransformComponent* transform;

	AttackComponent() = default;

	~AttackComponent() {
		SDL_DestroyTexture(texture);
	}

	void init() override;

	void update() override;
	void render() override;

	void attack();
};