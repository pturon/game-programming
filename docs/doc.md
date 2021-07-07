# Blog

## main.cpp

Die main.cpp initialisiert ein Objekt der Game Klasse und startet damit das Spiel.

 Zudem wird hier der Game-Loop gestartet und die Framerate auf 60 FPS gesetzt. 

```c++
#include "SDL.h"
#include "include/Game.h"
#include "include/Constants.h"

const int FPS = 60;
const int frameDelay = 1000 / FPS;

Game* game = nullptr;

int main(int argc, char* argv[]) {

	Uint32 frameStart;
	int frameTime; 

	game = new Game(); 
	game->init("Metroidvania", WINDOW_WIDTH, WINDOW_HEIGHT, false);

	while (game->running()) {

		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();

	return 0;
}
```



## Game.h und .cpp

Die Game-Klasse beinhaltet alle relevanten Elemente der Spielelogik. 

Sie enthält einen Entity-Manager und sorgt dafür das alle Entitys geupdated und gezeichnet werden. Außerdem wird auf den Input des Spielers reagiert und dieser wird gespeichert. 

In der Update-Methode wird geachtet, dass es keine Kollisionen vom Spieler und der Spielwelt gibt. 

Ein Kamera-Objekt simuliert eine Kamera und ermöglicht ein flüssiges scrollen der Spielwelt in alle Richtungen. (Wird eventuell noch verändert)

**Code folgt, wenn fehlerfrei**



## EntityComponentSystem.h und .cpp

Die Architektur der verschiedenen Objekte im Spiel wird mit einem Entity-Component-System realisiert. Jedes Objekt ist ein Entity. Ein Entity wird durch seine gespeicherten Components definiert. Jedes Component steht dabei für eine Fähigkeit, die das Entity hat, wie z.B einen Sprite oder eine Tastatursteuerung. Alle Entities werden in einem Entity-Manager gespeichert. 

Die **Component-Klasse** ist eine abstrakte Klasse von der alle näher definierten Components erben. Diese stellt einen Verweis auf das Entity, in dem das Component sich befindet, eine Initialisierungs-Funktion, eine Update-Funktion und eine Zeichen-Funktion bereit. 

Die **Entity-Klasse** besteht hauptsächlich aus einer Liste an Components und einem Verweis auf den Manager, in dem das Entity gespeichert wird. Ein Enity kann nur ein Component von jedem Typ enthalten. In der Update- und der Render-Funktion wird die jeweilige Methode einmal in allen gespeicherten Components aufgerufen. Die Variable active sagt aus, ob das Entity bereit ist vom Manager gelöscht  zu werden.

Die **Manager-Klasse** speichert zum einen alle hinzugefügten Entitys und eine Liste aller Gruppen und zu jeder Gruppe die Entities, die dieser Gruppe zugeordnet werden. Die Gruppen machen es einfacher den Typ eines Entitys zu erkennen und bestimmt die Reihenfolge in der die Entitys gezeichnet werden. Der Manager kann Entities hinzugügen und überprüft jeden Tick ob ein gespeichertes Entity zur Löschung bereit ist. Falls ein Entity bereit ist, wird dieses gelöscht. Alle Entities werden durch den Manager der Reihe nach geupdatet und die Gruppen werden von hinten nach vorne alle gezeichnet. 

```c++
#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "Enums.h"

//Forward declaration of classes
class Component;
class Entity;
class Manager; 

//Define ComponentID type as size_t
using ComponentID = std::size_t;

//Define Group type as size_t
using Group = std::size_t;

//Returns smallest unused ID for new Component Types
inline ComponentID getNewComponentTypeID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

//Returns ID of on existing Component Type
template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

//Maximum amount of Components that Entity can hold
constexpr std::size_t maxComponents = 32;
//Maximum amount of Groups 
constexpr std::size_t maxGroups = 32; 
//Defines variable that indicates if Entity stores specific Component
using ComponentBitSet = std::bitset <maxComponents>;
//
using GroupBitset = std::bitset<maxGroups>;
//Defines Array to store Components with length of maxComponents
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
	//Reference to parent
	Entity* parent;

	//Declarate abstract mehthods
	virtual void init() {}
	virtual void update() {}
	virtual void render() {}
	virtual ~Component() {}
private:
};

class Entity {
private:
	//If false Entity can be removed
	bool active = true;

	std::vector<std::unique_ptr<Component>> components;
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitset groupBitset;

	Manager& manager;
public:
	Entity(Manager& m) : manager(m) {}

	//Update all Components and call render function 
	void update() {
		for (auto& c : components) c->update();

	}
	void render() {
		for (auto& c : components) c->render();
	}

	//Returns active variable
	const bool isActive() { return active; }

	//Marks Entity to be deleted
	void destroy() { active = false; }

	//Returns true if Entity has Component T
	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}

	//Adds all given Components to Entity. Every Component Type is always on the same postion in the array
	template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
		//Loop through all Arguments
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->parent = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		//Add Component to Array
		componentArray[getComponentTypeID<T>()] = c;
		//Mark that Entity has Component
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	//Returns Component with the given type
	template<typename T> T& getComponent() {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	bool hasGroup(Group g) {
		return groupBitset[g];
	}

	void addGroup(Group g);
	void removeGroup(Group g) {
		groupBitset[g] = false; 
	}

};

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
public:
	//Updates all stored Entities 
	void update() {
		for (auto& e : entities) e->update();
	}
	//Renders all stored Entites 
	void render() {
		for (int l = groupBackground; l != groupForeground; l++) {
			auto& g = getGroup(l);
			for (auto& e : g) {
				e->render();
			}
		}
	}

	//Remove all inactive Entities or Entities without Group
	void refresh() {
		for (auto i(0u); i < maxGroups; i++) {
			auto& v(groupedEntities[i]);
			v.erase(std::remove_if(std::begin(v), std::end(v), [i](Entity* e) {
				return !e->isActive() || !e->hasGroup(i);
				}), std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity>& mEntity) {
			return !mEntity->isActive();
			}), std::end(entities));
	}

	//Adds Entity to Group
	void addToGroup(Entity* e, Group g) {
		groupedEntities[g].emplace_back(e);
	}

	//Returns List of Entities of Group
	std::vector<Entity*>& getGroup(Group g) {
		return groupedEntities[g];
	}

	//Adds Entity to Manager
	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};
```

```c++
#include "../include/EntityComponentSystem.h"

void Entity::addGroup(Group g) {
	groupBitset[g] = true; 
	manager.addToGroup(this, g);
}
```



## Vector2D.h und .cpp

Dee Vector2D Klasse simuliert einen zweidimensionalen Vektor. Hier werden die mathematischen Operatoren +,-,* und / für die Klasse überladen, um mit den Vektoren rechnen zu können.

```c++
class Vector2D {
public:
	float x; 
	float y; 

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& add(const Vector2D& v);
	Vector2D& sub(const Vector2D& v);
	Vector2D& mul(const Vector2D& v);
	Vector2D& div(const Vector2D& v);

	friend Vector2D& operator+(Vector2D& v1, Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, Vector2D& v2);

	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator*=(const Vector2D& v);
	Vector2D& operator/=(const Vector2D& v);

	Vector2D& operator*(const int& i);
	Vector2D& zero();
};
```

```c++
#include "../include/Vector2D.h"

Vector2D::Vector2D() {
	x = y = 0;
}

Vector2D::Vector2D(float x, float y) {
	this->x = x; 
	this->y = y; 
}

Vector2D& Vector2D::add(const Vector2D& v) {
	this->x += v.x;
	this->y += v.y;
	return *this; 
}
Vector2D& Vector2D::sub(const Vector2D& v) {
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}
Vector2D& Vector2D::mul(const Vector2D& v) {
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}
Vector2D& Vector2D::div(const Vector2D& v) {
	this->x /= v.x;
	this->y /= v.y;
	return *this;
}

Vector2D& operator+(Vector2D& v1, Vector2D& v2) {
	return v1.add(v2);
}
Vector2D& operator-(Vector2D& v1, Vector2D& v2) {
	return v1.sub(v2);
}
Vector2D& operator*(Vector2D& v1, Vector2D& v2) {
	return v1.mul(v2);
}
Vector2D& operator/(Vector2D& v1, Vector2D& v2) {
	return v1.div(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& v) {
	return this->add(v);
}
Vector2D& Vector2D::operator-=(const Vector2D& v) {
	return this->sub(v);
}
Vector2D& Vector2D::operator*=(const Vector2D& v) {
	return this->mul(v);
}
Vector2D& Vector2D::operator/=(const Vector2D& v) {
	return this->div(v);
}

Vector2D& Vector2D::operator*(const int& i) {
	this->x *= i;
	this->y *= i;
	return *this; 
}

Vector2D& Vector2D::zero() {
	this->x = 0;
	this->y = 0;
	return *this; 
}

```



## Components

Um ein Entity mit Leben zu füllen, werden viele verschiedene Components erstellt. Diese haben die unterschiedlichsten Fähigkeiten. 

## TransformComponent.h 

Das TransformComponent stellt die physische Komponente des Objektes dar. Hier wird die aktuelle Position und die Größe des Objektes gespeichert. Für bewegliche Objekte wird hier auf die Geschwindigkeit und die aktuelle Bewegungsrichtung gespeichert. 

In der Update-Methode wird die nächste Position des Objektes berechnet. Dabei wird die Schwerkraft oder ob das Objekt am Springen ist mit einbezogen.

Das **Springen**: Die Sprungkurve der Spielfigur wird durch eine Parabel dargestellt d.h. die Steigung der Kurve wird immer um den gleichen Wert weniger. Da ein physikalisch korrekter Sprung sich im Hinblick auf das Spielgefühl zu langsam anfühlt, wird die Schwerkraft beim Fallen erhöht, damit die Spielfigur schneller fällt als sie steigt. Um mit der Dauer in der die Leertaste gedrückt wird, die Höhe des Sprunges steuern zu können, wird die Schwerkraft um einen kleinen Wert erhöht, solange die Leertaste nicht gedrückt wird. 

```c++
#pragma once
#include "Components.h"
#include "Vector2D.h"
#include "SDL.h"

class TransformComponent : public Component {
public:

	Vector2D position;
	Vector2D velocity;
	Vector2D gravity; 
	bool gravityAffected = false;
	int speed = 5; 

	bool jumping = false; 
	bool fallingAfterJump = false; 
	float lastTick = 0; 
	int jumpHeight = 60;

	float fallMultiplier = 3.5f;
	float lowJumpMultiplier = 1.5f; 
	bool spaceDown = false; 

	int width = 32;
	int height = 32; 
	int scale = 1; 

	TransformComponent() {
		position.zero();
	}
	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(int s) {
		position.zero();
		scale = s; 
	}

	TransformComponent(float x, float y, int w, int h, int s, int sp, bool g) {
		position.x = x;
		position.y = y;
		width = w; 
		height = h; 
		scale = s; 
		speed = sp;
		gravityAffected = g; 
		gravity = { 0,9 };
	}

	TransformComponent(float x, float y, int w, int h) {
		position.x = x;
		position.y = y;
		width = w;
		height = h;	
	}

	void init() override {
		velocity.zero();
	}

	void update() override; 

	void jump() {
		if (!jumping && !fallingAfterJump) {
			jumping = true;
			velocity.y = static_cast<float>(-jumpHeight);			
		}		
	}

	void stopJump() {
		if (fallingAfterJump || jumping) {
			jumping = false;
			fallingAfterJump = false;
			velocity.y = 0; 
		}		
	}

	void startFall() {
		jumping = false; 
		fallingAfterJump = true;
		velocity.y = 0;
	}
};
```

```c++
#include "../include/TransformComponent.h"

void TransformComponent::update() {
	if (velocity.y > 0) {
		velocity.y += fallMultiplier;
	}
	else if (velocity.y < 0 && !spaceDown) {
		velocity.y += lowJumpMultiplier;
	}
	if (jumping) {
		float t = 0.16f;
		position.x += velocity.x * speed;
		position.y += t * velocity.y;
		velocity.x += t * (gravity.x * speed);
		velocity.y += t * gravity.y;

		std::cout << "jump" << std::endl;
	}
	else {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
		if (gravityAffected) {
			position.x += gravity.x;
			position.y += gravity.y;
		}
	}
}
```



## SpriteComponent.h und .cpp

Das SpriteComponent ist die visuelle Komponente des Entities. Hier wird das Aussehen definiert und alle Animationen gespeichert. 

Für Objekte ohne Animationen wird in der Update-Methode nur die Position zur aktuellen des Objektes in Abhängigkeit zum gezeigten Bildausschnitt geupdated.

Für animierte Sprites wird zusätzlich  jedes mal der aktuelle Frame der Animation ausgewählt. Welche Animation abgespielt wird, entscheidet die Variable **animationIndex**. Diese gibt die Zeile der Animation auf dem Spritesheet an. Die Dauer eines Frames gibt die Variable **speed** an. 

```c++
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
```

```c++
#include "../include/SpriteComponent.h"


SpriteComponent::SpriteComponent(const char *path) {
	texture = TextureManager::loadTexture(path);
}

SpriteComponent::SpriteComponent(const char* path, bool isAnimated) {
	animated = isAnimated; 

	Animation idle = Animation(0, 2, 100);
	Animation walking = Animation(1, 4, 100);

	animations.emplace("Idle", idle);
	animations.emplace("Walking", walking);

	switchAnimation("Idle");

	texture = TextureManager::loadTexture(path);
}

SpriteComponent::~SpriteComponent() {
	SDL_DestroyTexture(texture);
}

void SpriteComponent::init(){
	transform = &parent->getComponent<TransformComponent>();
	srcRect.x = srcRect.y = 0;
	srcRect.w = transform->width;
	srcRect.h = transform->height;	
}

void SpriteComponent::update() {

	if (animated) {
		srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		srcRect.y = srcRect.h * animationIndex;
	}

	srcRect.y = animationIndex * transform->height;

	destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	destRect.w = transform->width * transform->scale;
	destRect.h = transform->height * transform->scale;
}

void SpriteComponent::render() {
	destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	destRect.w = transform->width * transform->scale;
	destRect.h = transform->height * transform->scale;
	TextureManager::draw(texture, srcRect, destRect, spriteFlip);
}

void SpriteComponent::switchAnimation(const char* animationName) {
	frames = animations[animationName].frames; 
	animationIndex = animations[animationName].index;
	speed = animations[animationName].speed;
}

void SpriteComponent::flipAnimation(bool f) {
	if (f) {
		spriteFlip = SDL_FLIP_HORIZONTAL;
	}
	else {
		spriteFlip = SDL_FLIP_NONE;
	}
}

```



## KeyboardController.h und .cpp 

Der KeyboardController steuert die Bewegungen des Entities. Hier darauf reagiert, ob eine Taste auf der Tastatur gedrückt oder losgelassen wird. Bei den Taste A und D wird dem TransformComponent des Entitys signalisiert, dass sich die Figur nach Links oder Rechts bewegen soll. Beim drücken der Leertaste wird dem TransformComponent signalisiert, dass das Entity springen soll. Außerdem wird dem SpriteComponent des Entitys 

Bei einem Drück auf die Esc-Taste wird das Spiel beendet. 

```c++
#pragma once
#include "Game.h"
#include "EntityComponentSystem.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"

class SpriteComponent; 

class KeyboardController : public Component {
private:
	bool leftDown = false;
	bool rightDown = false;	
public: 
	TransformComponent *transform; 
	SpriteComponent* sprite; 
	bool spaceDown = false;

	void init() override;
	void update() override; 
	void getComponents();
};
```

```c++
#include "../include/KeyboardController.h"

void KeyboardController::init() {
	
}

void KeyboardController::getComponents() {
	transform = &parent->getComponent<TransformComponent>();
	sprite = &parent->getComponent<SpriteComponent>();
}

void KeyboardController::update() {
	if (Game::event.type == SDL_KEYDOWN) {
		switch (Game::event.key.keysym.sym) {
		case SDLK_w:
	
			break;
		case SDLK_a:
			leftDown = true;
			break;
		case SDLK_s:

			break;
		case SDLK_d:
			rightDown = true;
			break;
		case SDLK_SPACE:
			transform->jump();
			transform->spaceDown = true; 
			break;
		default:
			break;
		}
	}
	if (Game::event.type == SDL_KEYUP) {
		switch (Game::event.key.keysym.sym) {
		case SDLK_w:

			break;
		case SDLK_a:
			leftDown = false;
			break;
		case SDLK_s:

			break;
		case SDLK_d:
			rightDown = false; 
			break;
		case SDLK_ESCAPE:
			Game::isRunning = false; 
			break; 
		case SDLK_SPACE: 
			transform->spaceDown = false;
			break; 
		default:
			break;
		}
	}

	if (leftDown && !rightDown) {
		sprite->switchAnimation("Walking");
		sprite->flipAnimation(true);
		transform->velocity.x = -1;
	}
	else if (!leftDown && rightDown) {
		sprite->switchAnimation("Walking");
		sprite->flipAnimation(false);
		transform->velocity.x = 1;
	}
	else {
		transform->velocity.x = 0;
		sprite->switchAnimation("Idle");
	}
	
}
```



## ColliderComponent.h und .cpp

Physische Objekte, die mit anderen kollidieren sollen, bekommen ein ColliderComponent. Dies wird durch ein SDL_Rect realisiert, das immer auf die Position des TransformComponents gesetzt wird und die Größe des Entitys hat. 

Alle Kollisionsabfragen werden in der game.cpp gemacht. Das ColliderComponent ist nur eine Komponente, die einfach überprüft werden kann. 

```c++
#pragma once
#include "Components.h"
#include <string>
#include "SDL.h"
#include "TextureManager.h"
#include "Constants.h"

class TransformComponent; 

class ColliderComponent : public Component{
private:

public: 
	SDL_Rect collider;
	std::string tag;
	SDL_Rect srcRect, destRect;

	TransformComponent* transform;

	ColliderComponent(std::string t) {
		tag = t;
	}
	ColliderComponent(std::string t, int x, int y, int w, int h) {
		tag = t;
		collider.x = x; 
		collider.y = y; 
		collider.w = w; 
		collider.h = h; 
	}

	void init() override;
	void update() override;
};


```

```c++
#include "../include/ColliderComponent.h"

void ColliderComponent::init(){
	if (!parent->hasComponent<TransformComponent>()) {
		parent->addComponent<TransformComponent>();
	}
	transform = &parent->getComponent<TransformComponent>();
}

void ColliderComponent::update() {
	if (tag != "terrain") {		
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale; 
	}
	destRect.x = collider.x - Game::camera.x;
	destRect.y = collider.y - Game::camera.y;	
}
```



## Collision.h  und .cpp

Hier werden alle möglichen Funktionen aufgelistet, die berechnen, ob sich zwei Objekte überschneiden. Zur Zeit gibt es nur die Möglichkeit abzufragen, ob sich zwei Rechtecke überschneiden.

```c++
#pragma once
#include "SDL.h"

class ColliderComponent; 

class Collision{
public: 
	static bool AABB(const SDL_Rect& a, const SDL_Rect& b);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};
```

```c++
#include "../include/Collision.h"
#include "../include/ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& a, const SDL_Rect& b) {
	if (a.x + a.w >= b.x && b.x + b.w >= a.x && a.y + a.h >= b.y && b.y + b.h >= a.y) {
		return true; 
	}
	return false; 
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	return AABB(colA.collider, colB.collider);
}
```



## Kollisionsabfrage

Damit die Spielfigur nicht durch die Spielwelt fällt, wird bei jedem Tick abgefragt ob der Collider der Spielfigur mit einem anderen Collider kollidiert. Falls dies der Fall sein sollte, wird die Position der Figur korrigiert. 

Leider reicht es nicht nur zu überprüfen, ob sich die Collider überschneiden, muss zuerst die Position der Spielfigur zum überschnittenen Collider überprüft werden. 

```c++
for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {					
			if (cCol.y < playerCol.y + PLAYER_HEIGHT && playerCol.y < cCol.y + cCol.h) {
				//right 
				if (playerCol.x + playerCol.w - cCol.x >= 0 && playerCol.x + playerCol.w - cCol.x < 10) {
					playerCol.x = cCol.x - PLAYER_WIDTH - 1;					
				}
				//left 
				if (playerCol.x > cCol.x && playerCol.x < cCol.x + cCol.w) {
					playerCol.x = cCol.x + cCol.w + 1;					
				}
			}
			//above
			if (playerCol.y + PLAYER_HEIGHT > cCol.y && playerCol.y + PLAYER_HEIGHT < cCol.y + cCol.h && 	playerCol.y + PLAYER_HEIGHT - cCol.y <= yDist) {
				playerCol.y = cCol.y - PLAYER_HEIGHT - 1;
				player.getComponent<TransformComponent>().stopJump();
			}
			//below 
			if (playerCol.y > cCol.y && playerCol.y < cCol.y + cCol.h && cCol.y + cCol.h - playerCol.y <= yDist) {
				playerCol.y = cCol.y + cCol.h + 1;
				player.getComponent<TransformComponent>().startFall();
			}
		}
	}
	player.getComponent<TransformComponent>().position.x = playerCol.x;
	player.getComponent<TransformComponent>().position.y = playerCol.y;
```

