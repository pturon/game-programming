#pragma once
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"

class SpriteComponent : public Component {
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false; 
	int frames = 0; 
	int speed = 100; 

public:
	SpriteComponent() = default;
	SpriteComponent(const char* path);
	SpriteComponent(const char* path, int f, int s);
	~SpriteComponent();

	void init() override;
	void update() override;
	void render() override;
};