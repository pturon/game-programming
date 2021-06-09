#pragma once
#include "EntityComponentSystem.h"
#include "SDL.h"
#include <string>

class TransitionComponent : public Component {
public:
	SDL_Rect collider; 
	std::string level; 
	int newX, newY;

	TransitionComponent(int x, int y, int w, int h, std::string l, int nX, int nY) {
		collider = { x,y,w,h };
		level = l; 
		newX = nX;
		newY = nY; 
	}

};