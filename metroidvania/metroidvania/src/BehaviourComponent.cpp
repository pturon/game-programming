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

void BehaviourComponent::onHit(Direction d) {
	for (auto& b : behaviours) b->onHit(d);
}

void BehaviourComponent::die() {
	for (auto& b : behaviours) b->die();
}
