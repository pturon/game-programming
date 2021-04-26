#include "../include/TileMap.h"
#include "../include/Game.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

TileMap::TileMap() {	
	width = 0; 
	height = 0; 
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
				Game::addTile(srcX, srcY, x * TILE_WIDTH, y * TILE_HEIGHT);
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



