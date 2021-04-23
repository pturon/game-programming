#pragma once
#include "EntityComponentSystem.h"

class PositionComponent : public Component {
private:
	int posX, posY;
public:
	int x() { return posX; }
	int y() { return posY; }

	void setPos(int x, int y) {
		posX = x; 
		posY = y; 
	}

	void init() override {
		posX = 0; 
		posY = 0; 
	}

	void update() override {
		posX++;
		posY++;
	}
};