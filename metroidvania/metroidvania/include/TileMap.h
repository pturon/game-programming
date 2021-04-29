#pragma once

#include "SDL.h"
#include <string>
#include "TileComponent.h"
#include "ColliderComponent.h"
#include "EntityComponentSystem.h"
#include "Enums.h"

class TileComponent; 
class ColliderComponent; 

class TileMap{
public:
	TileMap(const char* tileSet);
	~TileMap();

	int width; 
	int height;

	void loadMap(std::string path);
	void loadColliders(std::string path);
	void addTile(int srcX, int srcY, int x, int y);
	void addCollider(int x, int y);
private:
	const char* mapFilePath; 
	const char* colliderFilePath; 
	const char* tileSetPath;
	void setWidth(int w) {
		width = w;
	}
	void setHeight(int h) {
		height = h;
	}
};

