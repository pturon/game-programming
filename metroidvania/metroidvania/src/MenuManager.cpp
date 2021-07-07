#include "../include/MenuManager.h"



void MenuManager::showDeathMenu() {
	currentMenu = death;
}

void MenuManager::showWinMenu() {
	currentMenu = win;
}

void MenuManager::init() {
	currentMenu = noMenu; 
	font = TTF_OpenFont("assets/Bebas-Regular.ttf", 64);
	color = { 255,255,255 };
	background = TextureManager::loadTexture("assets/menu_background.png");
}

void MenuManager::update() {
	switch (currentMenu)	{
	case win: 
		break; 
	case death: 
		break; 
	case none: 
		break; 
	default:
		break;
	}
}

void MenuManager::render() {
	if (currentMenu == win) {		
		SDL_Rect destRect = { 0,0,WINDOW_WIDTH, WINDOW_HEIGHT };
		SDL_Rect srcRect = { 0,0,32,32 };
		TextureManager::draw(background, srcRect, destRect, SDL_FLIP_NONE);
		destRect = { 280,200 ,WINDOW_WIDTH, WINDOW_HEIGHT };
		std::string str = "Gewonnen";
		char* char_type = (char*)str.c_str();
		TextureManager::drawText(char_type, destRect, font, color);
	}
	else if (currentMenu == death) {
		SDL_Rect destRect = { 0,0,WINDOW_WIDTH, WINDOW_HEIGHT };
		SDL_Rect srcRect = { 0,0,32,32 };
		TextureManager::draw(background, srcRect, destRect, SDL_FLIP_NONE);
		destRect = { 290,200 ,WINDOW_WIDTH, WINDOW_HEIGHT };
		std::string str = "Verloren";
		char* char_type = (char*)str.c_str();
		TextureManager::drawText(char_type, destRect, font, color);
	}
}