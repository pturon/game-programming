#pragma once
#include "Components.h"
#include "Vector2D.h"
#include "SDL.h"
#include "Enums.h"

class StateComponent; 

class TransformComponent : public Component {
public:

	StateComponent* state; 

	Vector2D position;
	Vector2D velocity;
	Vector2D gravity; 
	bool gravityAffected = false;
	int speed = 5; 

	bool canDash = true;
	int dashSpeed = 20; 
	int dashDuration = 200; 
	int dashStart = 0; 

	bool canDoubleJump = true; 
	bool fallingAfterJump = false; 
	float lastTick = 0; 
	int jumpHeight = 60;

	float fallMultiplier = 3.5f;
	float lowJumpMultiplier = 1.5f; 
	bool spaceDown = false; 

	Direction direction;

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

	TransformComponent(float x, float y, int w, int h, int s, int sp, bool g) {
		position.x = x;
		position.y = y;
		width = w; 
		height = h; 
		scale = s; 
		speed = sp;
		gravityAffected = g; 
		gravity = { 0,9 };
	}

	TransformComponent(float x, float y, int w, int h) {
		position.x = x;
		position.y = y;
		width = w;
		height = h;	
	}

	void init() override;

	void update() override; 

	void jump();

	void stopJump();

	void startFall();

	void moveLeft();
	void moveRight();
	void moveStop(); 

	void dash();
	void stopDash();
};