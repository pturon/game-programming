#pragma once

#include "SDL.h"
#include <string>

class TileMap{
public:
	TileMap();
	~TileMap();

	static void loadMap(std::string path);
private:
};

