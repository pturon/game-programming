#pragma once
#include "EntityComponentSystem.h"
#include "StatsComponent.h"
#include "TextureManager.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <sstream>
#include <string.h>

class StatsComponent; 

class HudManager{
public:
	StatsComponent* playerStats;

	SDL_Texture* fullHeart;
	SDL_Texture* emptyHeart;
	SDL_Texture* moneyIcon; 
	SDL_Texture* manaTank; 
	SDL_Texture* mana; 

	HudManager() = default;
	~HudManager();

	void init();
	void update();
	void render();
};

