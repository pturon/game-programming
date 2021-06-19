#include "../include/BehaviourComponent.h"

BehaviourComponent::~BehaviourComponent() {
}

void BehaviourComponent::setBehaviour(Behaviour b) {
	behaviour = &b;
	behaviour->parent = parent;
}

void BehaviourComponent::init() {	
	behaviour->init();
}

void BehaviourComponent::update() {
	behaviour->update();
}

void BehaviourComponent::onCollision(Vector2D cn) {
	behaviour->onCollision(cn);
}

void BehaviourComponent::onHit() {
	behaviour->onHit();
}
