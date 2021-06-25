#include "../include/GoombaBehaviour.h"

void GoombaBehaviour::init() {
	parent->getComponent<TransformComponent>().velocity = { 1,0 };
}

void GoombaBehaviour::update() {	
	tickCounter = SDL_GetTicks();	
	if (parent->getComponent<StateComponent>().currentState == dying) {				
		if (tickCounter - tickStart >= dyingDuration) {			
			tickStart = SDL_GetTicks();
			parent->getComponent<StateComponent>().setState(dead);
			
		}
	}
	if (parent->getComponent<StateComponent>().currentState == dead) {
		if (tickCounter - tickStart >= despawnTime) {
			parent->destroy();
		}
	}
}

void GoombaBehaviour::onCollision(Vector2D cn) {
	if (cn.x != 0 && cn.y == 0) {
		parent->getComponent<TransformComponent>().velocity = cn;
		if (cn.x == 1) {
			parent->getComponent<SpriteComponent>().flipAnimation(false);
		}
		else {
			parent->getComponent<SpriteComponent>().flipAnimation(true);
		}		
	}
}

void GoombaBehaviour::onHit(Direction d) {
	Vector2D recoilV = { 0,0 };
	switch (d) {
	case up:
		recoilV.y = -1;
		break;
	case right:
		recoilV.x = 1; 
		break; 
	case down:
		recoilV.y = 1; 
		break; 
	case left: 
		recoilV.x = -1; 
		break;
	default:
		break; 
	}
	parent->getComponent<TransformComponent>().startRecoil(recoilV);
}

void GoombaBehaviour::die() {
	if (parent->getComponent<StateComponent>().currentState != dying && parent->getComponent<StateComponent>().currentState != dead) {
		parent->getComponent<StateComponent>().setState(dying);
		tickCounter = SDL_GetTicks();
		parent->getComponent<TransformComponent>().velocity.x = 0;
	}	
}