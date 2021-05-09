#pragma once
#include "Game.h"
#include "EntityComponentSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "AttackComponent.h"
#include "StateComponent.h"
#include "Enums.h"

class SpriteComponent; 
class StateComponent; 

class KeyboardController : public Component {
private:
	bool leftDown = false;
	bool rightDown = false;	
public: 
	TransformComponent *transform; 
	SpriteComponent* sprite; 
	StateComponent* state; 
	bool spaceDown = false;

	void init() override;
	void update() override; 
	void getComponents();
};