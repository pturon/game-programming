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

bool Collision::PointRect(const Vector2D& p, const SDL_Rect& r) {
	return (p.x >= r.x && p.y >= r.y && p.x < r.x + r.w && p.y < r.y + r.h);
}

bool Collision::RectRect(const SDL_Rect& r1, const SDL_Rect& r2) {
	return (r1.x < r2.x + r2.w && r1.x + r1.w > r2.x && r1.y < r2.y + r2.h && r1.y + r1.h > r2.y);
}

bool Collision::RectRect(const ColliderComponent& r1, const ColliderComponent& r2) {
	return RectRect(r1.collider, r1.collider);
}