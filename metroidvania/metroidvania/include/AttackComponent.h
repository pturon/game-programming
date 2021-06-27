#pragma once

#include "EntityComponentSystem.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "StateComponent.h"
#include "SDL.h"
#include "Enums.h"

class TransformComponent;
class AttackComponent; 
class StateComponent; 

class AttackComponent : public Component {
private: 	
	int attackDuration = 400; 
	int attackStart = 0; 
	int lastTick = 0; 
	SDL_Texture* texture; 
	SDL_Rect destRect; 
public: 
	StateComponent* state; 
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