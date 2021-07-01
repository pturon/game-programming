#pragma once
#include "BehaviourComponent.h"
#include "Constants.h"
#include "TransformComponent.h"
#include "Enums.h"

class SkeletonBehaviour : public Behaviour{
public: 
	int detectionRange = 300; 
	int attackRange = 32; 
	int attackDuration = 600;
	int cooldown = 1000;
	int attackCharge = 500; 
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

