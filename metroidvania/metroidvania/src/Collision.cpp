#include "../include/Collision.h"
#include "../include/ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& a, const SDL_Rect& b) {
	if (a.x + a.w >= b.x && b.x + b.w >= a.x && a.y + a.h >= b.y && b.y + b.h >= a.y) {
		return true; 
	}
	return false; 
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	return AABB(colA.collider, colB.collider);
}