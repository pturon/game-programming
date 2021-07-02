#include "../include/ColliderComponent.h"

void ColliderComponent::init(){
	if (!parent->hasComponent<TransformComponent>()) {
		parent->addComponent<TransformComponent>();
	}
	transform = &parent->getComponent<TransformComponent>();

	texture = TextureManager::loadTexture("assets/c.png");

	if (tag != "terrain") {
		collider.x = static_cast<int>(transform->position.x + xOffset);
		collider.y = static_cast<int>(transform->position.y + yOffset);
		collider.w = (transform->width - 2 * xOffset) * transform->scale;
		collider.h = (transform->height - 2 * yOffset) * transform->scale;
		lastPos = collider;
	}	

	srcRect.x = srcRect.y = 0; 
	srcRect.w = TILE_WIDTH;
	srcRect.h = TILE_HEIGHT;
	destRect = { collider.x, collider.y, collider.w, collider.h };
}

void ColliderComponent::update() {
	lastPos = collider;
	if (tag != "terrain") {			
		collider.x = static_cast<int>(transform->position.x + xOffset);
		collider.y = static_cast<int>(transform->position.y + yOffset);
		collider.w = (transform->width - 2 * xOffset) * transform->scale;
		collider.h = (transform->height - 2 * yOffset) * transform->scale;

	}
	destRect.x = collider.x - Game::camera.x;
	destRect.y = collider.y - Game::camera.y;	
}

void ColliderComponent::render() {	
		//TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);	
}