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
#include "HudManager.h"
#include "Map.h"
#include "Transition.h"
#include "Behaviour.h"
#include "MenuManager.h"


class ColliderComponent; 

class Game{
public:

	static bool isRunning;

	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();

	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

	static Entity& player; 
	static SDL_Renderer* renderer;
	static SDL_Event event; 

	static SDL_Rect camera; 

	static bool rayHitsCollider(Vector2D origin, Vector2D ray);

	static void end(); 

private:
	
	SDL_Window* window;

	
};



