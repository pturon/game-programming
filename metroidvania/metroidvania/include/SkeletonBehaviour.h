#pragma once
#include "BehaviourComponent.h"
#include "Constants.h"
#include "TransformComponent.h"
#include "Enums.h"

class SkeletonBehaviour : public Behaviour{
public: 
	int detectionRange = 300; 
	void init() override;
	void update() override;
	void onCollision(Vector2D cn) override;
	void onHit(Direction d) override;
};

