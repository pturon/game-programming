#pragma once
#include "EntityComponentSystem.h"
#include "Vector2D.h"

class BehaviourComponent : public Component{

	BehaviourComponent() = default; 
	~BehaviourComponent();

	void init();
	void update();
	void onCollision(Vector2D cn);
	void onHit(); 

};

