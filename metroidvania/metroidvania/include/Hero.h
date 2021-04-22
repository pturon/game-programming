#ifndef Hero_h
#define Hero_h
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Hero{
public:
	Hero();
	~Hero();

	void init(SDL_Renderer* r);

	void update();
	void render();
	void clean();

private:
	SDL_Renderer* renderer;
	SDL_Texture* playerTexture;

	int posX; 
	int posY;
	int dX; 
	int dY; 
	int speed; 
	int state = 0; 

	SDL_Rect srcR, destR;

};

#endif

