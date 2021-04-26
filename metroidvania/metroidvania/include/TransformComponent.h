#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component {
public:

	Vector2D position;
	Vector2D velocity;
	int speed = 5; 

	int width = 32;
	int height = 32; 
	int scale = 1; 

	TransformComponent() {
		position.zero();
	}
	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(int s) {
		position.zero();
		scale = s; 
	}

	TransformComponent(float x, float y, int w, int h, int s, int sp) {
		position.x = x;
		position.y = y;
		width = w; 
		height = h; 
		scale = s; 
		speed = sp;
	}

	TransformComponent(float x, float y, int w, int h) {
		position.x = x;
		position.y = y;
		width = w;
		height = h;	
	}

	void init() override {
		velocity.zero();
	}

	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};