#pragma once
#include "EntityComponentSystem.h"
#include "Vector2D.h"
#include <vector>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

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
	std::vector<std::unique_ptr<Behaviour>> behaviours;

	BehaviourComponent() = default;
	~BehaviourComponent();

	void init() override;
	void update() override;
	void onCollision(Vector2D cn);
	void onHit(); 

	template <typename T, typename... TArgs> T& setBehaviour(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->parent = this->parent; 
		std::unique_ptr<Behaviour> uPtr{ c };
		behaviours.emplace_back(std::move(uPtr));
		c->init();
		return *c; 
	}
};

