#include "../include/Transition.h"

void Transition::fadeIn() {
	opacity = 0;
	SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, opacity);
	while (opacity <= 255) {
		SDL_RenderFillRect(Game::renderer, &rect);
		SDL_RenderPresent(Game::renderer);
		opacity += 255 / 60;
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, (uint8_t)opacity);
		SDL_Delay(1000 / 60);
	}
}

void Transition::fadeOut() {
	opacity = 255;
	SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, opacity);
	while (opacity >= 0) {
		SDL_RenderFillRect(Game::renderer, &rect);
		SDL_RenderPresent(Game::renderer);
		opacity -= 255 / 60;
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, (uint8_t)opacity);
		SDL_Delay(1000 / 60);
	}
}