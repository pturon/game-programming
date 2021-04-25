#include "../include/TileMap.h"
#include "../include/Game.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

TileMap::TileMap() {	
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
				srcX = (stoi(cell) % 10) * 32;
				srcY = (stoi(cell) / 10) * 32;
				Game::addTile(srcX, srcY, x * 32, y * 32);
			}
			x++;
		}
		x = 0; 
		y++;
	}
	
	mapFile.close();
	
}



