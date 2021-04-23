#pragma once
#include "Components.h"

class PositionComponent : public Component {
private:
	int posX, posY; 
public:
	PositionComponent() {
		posX = posY = 0; 
	}
	PositionComponent(int x, int y) {
		posX = x; 
		posY = y;
	}
	int x() { return posX; }
	int y() { return posY; }
	void setPos(int x, int y) {
		posX = x; 
		posY = y;
	}
};