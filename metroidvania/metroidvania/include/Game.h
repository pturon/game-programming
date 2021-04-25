#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include <stdio.h>
#include <iostream>
#include "TileMap.h"
#include "Components.h"
#include "Collision.h"
#include "Enums.h"

class ColliderComponent; 

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

	static void addTile(int id, int x, int y);

	static SDL_Renderer* renderer;
	static SDL_Event event; 
	static std::vector<ColliderComponent*>colliders; 

private:
	bool isRunning;
	SDL_Window* window;
};



