#include "../include/BehaviourComponent.h"

BehaviourComponent::~BehaviourComponent() {
}

void BehaviourComponent::init() {	
	for (auto& b : behaviours) b->init();
}

void BehaviourComponent::update() {
	for (auto& b : behaviours) b->update();
}

void BehaviourComponent::onCollision(Vector2D cn) {
	for (auto& b : behaviours) b->onCollision(cn);
}

void BehaviourComponent::onHit() {
	for (auto& b : behaviours) b->onHit();
}
