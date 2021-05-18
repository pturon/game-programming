#pragma once
#include "SDL.h"
#include "Vector2D.h"

class ColliderComponent; 

class Collision{
public: 
	static bool AABB(const SDL_Rect& a, const SDL_Rect& b);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
	static bool PointRect(const Vector2D& p, const SDL_Rect& r);
	static bool RectRect(const SDL_Rect& r1, const SDL_Rect& r2);
	static bool RectRect(const ColliderComponent& r1, const ColliderComponent& r2);
	static bool RayRect(const Vector2D& origin, const Vector2D& dir, const SDL_Rect& target, Vector2D& contactPoint, Vector2D& contactNormal, float& t_hit_near);
};

