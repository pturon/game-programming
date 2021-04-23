#pragma once
#include "Components.h"
#include <string>
#include "SDL.h"

class ColliderComponent : public Component{
private:

public: 
	SDL_Rect collider;
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t) {
		tag = t;
	}

	void init() override {
		if (!parent->hasComponent<TransformComponent>()) {
			parent->addComponent<TransformComponent>();
		}
		transform = &parent->getComponent<TransformComponent>();
	}

	void update() override {
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}
};

