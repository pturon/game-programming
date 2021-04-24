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

	void init() override;
	void update() override;
};

