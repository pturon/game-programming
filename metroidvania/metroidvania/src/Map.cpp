#include "../include/Map.h"
#include "../include/Game.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

extern Manager manager;

Map::~Map(){}

void Map::clearMap() {
	manager.clearGroup(groupBackground);
	manager.clearGroup(groupMidground);
	manager.clearGroup(groupForeground);
	manager.clearGroup(groupColliders);
	manager.clearGroup(groupTransitions);
}

void Map::loadMap(std::string level) {
	std::fstream mapFile("assets/levels/" + level +".map");
	std::string line;

	//Get Map Dimensions
	std::getline(mapFile, line);	
	width = stoi(line.substr(line.find(" ") + 1, line.length()));
	std::getline(mapFile, line);
	height = stoi(line.substr(line.find(" ") + 1, line.length()));

	//Skip unused lines
	for (int i = 0; i < 4; i++) {
		std::getline(mapFile, line);
	}	

	for (int layer = 0; layer < 4; layer++) {
		for (int y = 0; y < height; y++) {
			std::getline(mapFile, line);
			std::stringstream s(line);
			std::string cell;
			int x = 0; 
			int srcX, srcY;
			while (std::getline(s, cell, ',')) {
				if (cell != "-1" && x < width) {
					srcX = (stoi(cell) % TILESET_WIDTH) * TILE_WIDTH;
					srcY = (stoi(cell) / TILESET_WIDTH) * TILE_HEIGHT;
					if (layer < 3) {
						addTile(srcX, srcY, x * TILE_WIDTH, y * TILE_HEIGHT, layer);
					}
					else {
						addCollider(x, y);
					}
				}
				x++;
			}			
		}
		std::getline(mapFile, line);
		std::getline(mapFile, line);
	}

	mapFile.close();
	
	mapFile.open("assets/levels/" + level + "_transitions.map");
	while (std::getline(mapFile, line)) {
		std::cout << line << std::endl; 
		std::stringstream s(line);
		std::string cell;
		std::getline(s, cell, ',');
		int x = stoi(cell);
		std::getline(s, cell, ',');
		int y = stoi(cell);
		std::getline(s, cell, ',');
		int w = stoi(cell);
		std::getline(s, cell, ',');
		int h = stoi(cell);
		std::string level;
		std::getline(s, level, ',');
		std::getline(s, cell, ',');
		int newX = stoi(cell);
		std::getline(s, cell, ',');
		int newY = stoi(cell);
		addTransition(x, y, w, h, level, newX, newY);
	}
	mapFile.close();
}
 
void Map::addTile(int srcX, int srcY, int x, int y, int layer) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, x, y, tileSetPath);
	switch (layer)	{
	case 0:
		tile.addGroup(groupBackground);
		break;
	case 1:
		tile.addGroup(groupMidground);
		break;
	case 2:
		tile.addGroup(groupForeground);
		break;
	default:
		break;
	}	
}

void Map::addCollider(int x, int y) {
	auto& collider(manager.addEntity());
	collider.addComponent<ColliderComponent>("terrain", x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
	collider.addGroup(groupColliders);
}

void Map::addTransition(int x, int y, int w, int h, std::string l, int nX, int nY) {
	auto& transition(manager.addEntity());
	transition.addComponent<TransitionComponent>(x, y, w, h, l, nX, nY);
	transition.addGroup(groupTransitions);
}