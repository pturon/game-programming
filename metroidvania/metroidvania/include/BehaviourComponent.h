#pragma once
#include "EntityComponentSystem.h"
#include "Vector2D.h"

class Behaviour {
public:
	Entity* parent;
	virtual void init() {};
	virtual void update() {};
	virtual void onCollision(Vector2D cn) {}
	virtual void onHit() {};
};

class BehaviourComponent : public Component{
public: 
	Behaviour* behaviour; 

	BehaviourComponent() = default;
	~BehaviourComponent();

	void setBehaviour(Behaviour b);
	void init() override;
	void update() override;
	void onCollision(Vector2D cn);
	void onHit(); 

};

