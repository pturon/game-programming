#pragma once
#include "BehaviourComponent.h"
#include "TransformComponent.h"
#include "Enums.h"

class GoombaBehaviour : public Behaviour{
public:

	void init() override;
	void update() override;
	void onCollision(Vector2D cn) override;
	void onHit(Direction d) override;
};

