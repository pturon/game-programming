#include "../include/ColliderComponent.h"

void ColliderComponent::init(){
	if (!parent->hasComponent<TransformComponent>()) {
		parent->addComponent<TransformComponent>();
	}
	transform = &parent->getComponent<TransformComponent>();

	texture = TextureManager::loadTexture("assets/collider.png");

	srcRect.x = srcRect.y = 0; 
	srcRect.w = TILE_WIDTH;
	srcRect.h = TILE_HEIGHT;
	destRect = { collider.x, collider.y, collider.w, collider.h };
}

void ColliderComponent::update() {
	if (tag != "terrain") {
		std::cout << "Collider update before" << collider.y << std::endl;
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale; 
		std::cout << "Collider update after" << collider.y << std::endl;
	}
	destRect.x = collider.x - Game::camera.x;
	destRect.y = collider.y - Game::camera.y;

	
}