#include "../include/ColliderComponent.h"

void ColliderComponent::init(){
	if (!parent->hasComponent<TransformComponent>()) {
		parent->addComponent<TransformComponent>();
	}
	transform = &parent->getComponent<TransformComponent>();
	Game::colliders.push_back(this);
}

void ColliderComponent::update() {
	collider.x = static_cast<int>(transform->position.x);
	collider.y = static_cast<int>(transform->position.y);
	collider.w = transform->width * transform->scale;
	collider.h = transform->height * transform->scale;
}