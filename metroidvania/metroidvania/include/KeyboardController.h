#pragma once
#include "Game.h"
#include "EntityComponentSystem.h"
#include "Components.h"

class KeyboardController : public Component {
private:
	bool leftDown, rightDown = false;
public: 
	TransformComponent *transform; 

	void init() override;
	void update() override; 
};