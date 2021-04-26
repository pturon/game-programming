#pragma once

#include "SDL.h"
#include <string>

class TileMap{
public:
	TileMap();
	~TileMap();

	int width; 
	int height;

	void loadMap(std::string path);
private:
	void setWidth(int w) {
		width = w;
	}
	void setHeight(int h) {
		height = h;
	}
};

