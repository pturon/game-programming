#include "../include/TileMap.h"
#include "../include/Game.h"
#include <fstream>

TileMap::TileMap() {	
}

TileMap::~TileMap() {
}

void TileMap::loadMap(std::string path, int w, int h){
	char tile;
	std::fstream mapFile; 
	mapFile.open(path);

	for (int y = 0; y <= h; y++) {
		for (int x = 0; x <= w; x++) {
			mapFile.get(tile);
			Game::addTile(atoi(&tile),x * 32, y *32);
			mapFile.ignore();
		} 
	}

	mapFile.close();
}



