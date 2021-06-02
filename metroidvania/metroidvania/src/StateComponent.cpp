#include "../include/StateComponent.h"

void StateComponent::setState(State s) {
	if (s != currentState) {
		lastState = currentState;
		currentState = s;
		parent->getComponent<SpriteComponent>().switchAnimation(s);
	}	
}

void StateComponent::switchToLastState() {	
	State s = currentState;
	currentState = lastState;
	lastState = s;
	parent->getComponent<SpriteComponent>().switchAnimation(currentState);
}
