#pragma once

#include "Game.h"

class TileMap{
public:
	TileMap();
	~TileMap();

	void loadMap(int arr[20][25]);
	void render();
private:
	SDL_Rect srcRect, destRect;
	SDL_Texture* tileSet;

	int map[20][25];

	int tileSetW = 3; 
	int tileSetH = 3;
	
};

