#pragma once
#include "Game.h"
#include "EntityComponentSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

class SpriteComponent; 

class KeyboardController : public Component {
private:
	bool leftDown = false;
	bool rightDown = false;
public: 
	TransformComponent *transform; 
	SpriteComponent* sprite; 

	void init() override;
	void update() override; 
};