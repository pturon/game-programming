#include "../include/HudManager.h"

HudManager::~HudManager() {
	SDL_DestroyTexture(fullHeart);
	SDL_DestroyTexture(emptyHeart);
	SDL_DestroyTexture(moneyIcon);
}

void HudManager::init() {
	fullHeart = TextureManager::loadTexture("assets/heartFull.png");
	emptyHeart = TextureManager::loadTexture("assets/heartEmpty.png");
	moneyIcon = TextureManager::loadTexture("assets/moneyIcon.png");
	manaTank = TextureManager::loadTexture("assets/manaTank.png");
	mana = TextureManager::loadTexture("assets/mana.png");
}

void HudManager::update() {

}


void HudManager::render() {

	float manaHeight = 48 * static_cast<float>(playerStats->curMana) / static_cast<float>(playerStats->maxMana);

	SDL_Rect srcRect = { 0,48 - manaHeight,48,manaHeight};
	SDL_Rect destRect = { 10,10 + (48 - manaHeight),48,manaHeight };

	TextureManager::draw(mana, srcRect, destRect, SDL_FLIP_NONE);
	srcRect = { 0,0,48,48 };
	destRect = { 10,10,48,48 };
	TextureManager::draw(manaTank, srcRect, destRect, SDL_FLIP_NONE);

	srcRect = { 0,0,16,16 };
	destRect = { 58, 15, 16,16 };
	
	for (int i = 0; i < playerStats->maxHealth; i++) {
		if (i < playerStats->curHealth) {
			TextureManager::draw(fullHeart, srcRect, destRect, SDL_FLIP_NONE);
		}
		else {
			TextureManager::draw(emptyHeart, srcRect, destRect, SDL_FLIP_NONE);
		}
		destRect.x += 18; 
	}	
	destRect.x = 58;
	destRect.y += 18; 
	TextureManager::draw(moneyIcon, srcRect, destRect, SDL_FLIP_NONE);
	destRect.x += 18;
	destRect.y -= 3;
	std::stringstream strs;
	strs << playerStats->money;
	std::string temp_str = strs.str();
	char* char_type = (char*)temp_str.c_str();
	TextureManager::drawText(char_type, destRect);
}

