#ifndef Game_h
#define Game_h
#include "SDL.h"
#include "SDL_image.h"
#include "Hero.h"
#include <stdio.h>
#include <iostream>

class Game{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();

	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Hero* hero; 

};

#endif

