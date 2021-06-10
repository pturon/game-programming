#pragma once
#include "SDL.h"
#include "Constants.h"
#include "Game.h"

class Transition {
public: 
	bool shown = false; 
	float begin = 0.0f; 
	int duration = 500;
	SDL_Rect rect = { 0,0,WINDOW_WIDTH, WINDOW_HEIGHT };	
	int opacity = 0;
	SDL_Color color = { 0,0,0,0 };

	void fadeIn();
	void fadeOut();
};