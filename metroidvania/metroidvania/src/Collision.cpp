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

bool Collision::RayRect(const Vector2D& origin, const Vector2D& dir, const SDL_Rect& target, Vector2D &contactPoint, Vector2D &contactNormal, float &t_hit_near) {	
	Vector2D t_near = { (target.x - origin.x ) / dir.x, ((target.y - origin.y) / dir.y) };
	Vector2D t_far = { (target.x + target.w - origin.x) / dir.x, (target.y + target.y - origin.y) / dir.y };
	
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	if (t_near.x > t_far.y || t_near.y > t_far.x) return false; 

	t_hit_near = std::max(t_near.x, t_near.y);
	float t_hit_far = std::min(t_far.x, t_far.y);

	if (t_hit_far < 0) return false; 

	contactPoint = {(origin.x + dir.x * t_hit_near), (origin.y + dir.y * t_hit_near)};

	if (t_near.x > t_near.y) {
		if (dir.x < 0) {
			contactNormal = { 1, 0 };
		}
		else {
			contactNormal = { -1, 0 };
		}
	}
	else if (t_near.x < t_near.y) {
		if (dir.y < 0) {
			contactNormal = { 0,1 };
		}
		else {
			contactNormal = { 0,-1 };
		}
	}

	return true; 
}

bool Collision::DynamicRectRect(const SDL_Rect& in, const Vector2D inVelocity, const SDL_Rect& target, Vector2D& contactPoint, Vector2D& contactNormal, float& contactTime, float elapsedTime) {
	if (inVelocity.x == 0 && inVelocity.y == 0) {
		return false; 
	}
	SDL_Rect expandedTarget;
	expandedTarget.x = target.x - in.w / 2;
	expandedTarget.y = target.y - in.h / 2; 
	expandedTarget.w = target.w + in.w;
	expandedTarget.h = target.h + in.h;

	Vector2D centerPoint = { (in.x + in.w / 2), (in.y + in.h / 2) };

	if (RayRect(centerPoint, inVelocity, expandedTarget, contactPoint, contactNormal, contactTime)) {
		if (contactTime <= 1.0f) return true;
	}

	return false; 
}