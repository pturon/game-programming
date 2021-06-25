#pragma once
#include "BehaviourComponent.h"
#include "TransformComponent.h"
#include "Enums.h"
#include "SDL.h"

class GoombaBehaviour : public Behaviour{
public:
	int dyingDuration = 500;
	int despawnTime = 1000;
	int tickCounter = 0;
	int tickStart = 0; 
	void init() override;
	void update() override;
	void onCollision(Vector2D cn) override;
	void onHit(Direction d) override;
	void die() override;
};

