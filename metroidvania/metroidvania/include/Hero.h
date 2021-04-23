#pragma once
#include <iostream>
#include "Game.h"

class Hero{
public:
	Hero();
	~Hero();

	void update();
	void render();
	void clean();

private:
	SDL_Texture* playerTexture;
	SDL_Rect srcRect, destRect;

	int xpos; 
	int ypos; 
	

};


