#include "../include/TransformComponent.h"

void TransformComponent::update() {
	if (velocity.y > 0) {
		velocity.y += fallMultiplier;
	}
	else if (velocity.y < 0 && !spaceDown) {
		velocity.y += lowJumpMultiplier;
	}
	if (jumping) {
		float t = 0.16f;
		position.x += velocity.x * speed;
		position.y += t * velocity.y;
		velocity.x += t * (gravity.x * speed);
		velocity.y += t * gravity.y;

		std::cout << "jump" << std::endl;
	}
	else {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
		if (gravityAffected) {
			position.x += gravity.x;
			position.y += gravity.y;
		}
	}
}