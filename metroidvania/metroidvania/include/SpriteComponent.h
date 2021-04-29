#pragma once
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent : public Component {
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false; 
	int frames = 0; 
	int speed = 100; 

	std::map<const char*, Animation> animations;

public:
	int animationIndex = 0; 
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const char* path);
	SpriteComponent(const char* path, bool isAnimated);
	~SpriteComponent();

	void init() override;
	void update() override;
	void render() override;

	void switchAnimation(const char* animatioName);
	void flipAnimation(bool f);
};