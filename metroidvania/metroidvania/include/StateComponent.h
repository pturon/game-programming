#pragma once
#include "EntityComponentSystem.h"
#include "SpriteComponent.h"
#include "Enums.h"

class SpriteComponent; 

class StateComponent : public Component {
private:
public:
	State currentState = idle;
	State lastState = idle;
	StateComponent() = default; 
	void setState(State s);
	void switchToLastState() {
		State s = currentState;
		currentState = lastState;
		lastState = s; 
	}
	bool isAttacking() {
		return currentState == attackingBottom || currentState == attackingSide || currentState == attackingTop;
	}

};