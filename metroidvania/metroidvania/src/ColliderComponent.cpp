#include "../include/ColliderComponent.h"

void ColliderComponent::init(){
	if (!parent->hasComponent<TransformComponent>()) {
		parent->addComponent<TransformComponent>();
	}
	transform = &parent->getComponent<TransformComponent>();

	texture = TextureManager::loadTexture("assets/c.png");

	srcRect.x = srcRect.y = 0; 
	srcRect.w = TILE_WIDTH;
	srcRect.h = TILE_HEIGHT;
	destRect = { collider.x, collider.y, collider.w, collider.h };
}

void ColliderComponent::update() {
	if (tag != "terrain") {		
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale; 
	}
	destRect.x = collider.x - Game::camera.x;
	destRect.y = collider.y - Game::camera.y;	
}

void ColliderComponent::render() {
	
		//TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	
}