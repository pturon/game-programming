#pragma once
#include "BehaviourComponent.h"
#include "Constants.h"
#include "TransformComponent.h"
#include "Enums.h"

class SkeletonBehaviour : public Behaviour{
public: 
	int detectionRange = 300; 
	int attackRange = 32; 
	int attackDuration = 6;
	int cooldown = 60;
	int attackCharge = 30; 
	int tickCounter = 0; 
	void init() override;
	void update() override;
	void onCollision(Vector2D cn) override;
	void onHit(Direction d) override;
};

