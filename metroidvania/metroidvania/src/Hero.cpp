#include "../include/Hero.h"



Hero::Hero(SDL_Renderer* r) {
	renderer = r;
	playerTexture = TextureManager::loadTexture("assets/hero.png", renderer);
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
	SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);
}

void Hero::clean() {

}

