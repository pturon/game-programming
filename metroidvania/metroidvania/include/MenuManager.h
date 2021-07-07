#pragma once
#include "Enums.h"
#include "Game.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <sstream>
#include <string.h>
#include "TextureManager.h"

class MenuManager{
public:
	Menu currentMenu; 

	TTF_Font* font;
	SDL_Color color;
	SDL_Texture* background; 

	void showDeathMenu();
	void showWinMenu(); 

	void init();
	void update(); 
	void render(); 
};

