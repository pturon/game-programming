#pragma once
#include "SDL.h"
#include <string>
#include "TileComponent.h"
#include "ColliderComponent.h"
#include "EntityComponentSystem.h"
#include "Enums.h"
#include "Game.h"

class TileComponent;
class ColliderComponent;

class Map{
public: 
	const char* tileSetPath;
	int width = 0;
	int height = 0; 

	std::vector<TileComponent> tiles;

	Map() = default;
	~Map();

	void loadMap(std::string level);
	void clearMap();

	void addTile(int srcX, int srcY, int x, int y, int layer);
	void addCollider(int x, int y);
};

