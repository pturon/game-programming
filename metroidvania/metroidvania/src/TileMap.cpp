#include "../include/TileMap.h"
#include "../include/Game.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

extern Manager manager;

TileMap::TileMap(const char* tileSet) {
	width = 0; 
	height = 0; 
	tileSetPath = tileSet;
	mapFilePath = "";
	colliderFilePath = "";
}

TileMap::~TileMap() {
}

void TileMap::loadMap(std::string path){
	int srcX, srcY;
	
	std::fstream mapFile (path); 
	std::string line;	

	int x = 0;
	int y = 0;

	while (std::getline(mapFile, line)) {
		std::stringstream s(line);
		std::string cell; 
		while (std::getline(s, cell, ',')) {
			if (cell != "-1") {
				srcX = (stoi(cell) % TILESET_WIDTH) * TILE_WIDTH;
				srcY = (stoi(cell) / TILESET_WIDTH) * TILE_HEIGHT;
				addTile(srcX, srcY, x * TILE_WIDTH, y * TILE_HEIGHT);
			}
			x++;
		}
		if (x > width) {
			width = x; 
		}
		x = 0; 
		if (y > height) {
			height = y;
		}
		y++;		
	}
	
	mapFile.close();	
}

void TileMap::loadColliders(std::string path) {

	std::fstream mapFile(path);
	std::string line;

	char c; 

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			mapFile.get(c);
			if (c == '1') {				
				addCollider(x, y);
			}
			mapFile.ignore();
		}
	}
	mapFile.close();
}

void TileMap::addTile(int srcX, int srcY, int x, int y) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, x, y, tileSetPath);
	tile.addGroup(groupMap);
}

void TileMap::addCollider(int x, int y) {
	auto& collider(manager.addEntity());	
	collider.addComponent<ColliderComponent>("terrain", x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
	collider.addGroup(groupColliders);
}



