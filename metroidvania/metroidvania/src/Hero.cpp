#include "../include/Hero.h"

Hero::Hero() {	
	playerTexture = TextureManager::loadTexture("assets/hero.png");
}

Hero::~Hero() {

}

void Hero::update() {
	xpos = 0; 
	ypos = 0; 
	srcRect.h = 100; 
	srcRect.w = 100; 
	srcRect.x = 0; 
	srcRect.y = 0; 

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h; 
}

void Hero::render() {
	SDL_RenderCopy(Game::renderer, playerTexture, &srcRect, &destRect);
}

void Hero::clean() {

}

