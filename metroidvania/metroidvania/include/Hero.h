#ifndef Hero_h
#define Hero_h
#include <iostream>
#include "Game.h"

class Hero{
public:
	Hero(SDL_Renderer* r);
	~Hero();

	void update();
	void render();
	void clean();

private:
	SDL_Renderer* renderer;
	SDL_Texture* playerTexture;
	SDL_Rect srcRect, destRect;

	int xpos; 
	int ypos; 
	

};

#endif

