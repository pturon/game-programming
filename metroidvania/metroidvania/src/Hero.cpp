#include "../include/Hero.h"

Hero::Hero() {

}

Hero::~Hero() {

}

void Hero::init(SDL_Renderer* r) {
	renderer = r;

	destR.h = 100;
	destR.w = 100;

	SDL_Surface* tmpSurface = IMG_Load("../assets/Hero.png");
	playerTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
}

void Hero::update() {

}

void Hero::render() {
	SDL_RenderCopy(renderer, playerTexture, NULL, &destR);
}

void Hero::clean() {

}

