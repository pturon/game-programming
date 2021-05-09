#include "../include/StateComponent.h"

void StateComponent::setState(State s) {
	lastState = currentState;
	currentState = s;
	parent->getComponent<SpriteComponent>().switchAnimation(s);
}