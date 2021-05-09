#pragma once
#include "EntityComponentSystem.h"
#include "Enums.h"

class StateComponent : public Component {
private:
public:
	State currentState = idle;
	State lastState = idle;
	StateComponent() = default; 
	void setState(State s) {
		lastState = currentState;
		currentState = s; 
	}
	void switchToLastState() {
		State s = currentState;
		currentState = lastState;
		lastState = s; 
	}

};