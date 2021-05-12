# Woche 1 

## Zusammenfassung: 

In der ersten Woche wurde hauptsächlich das Grundgerüst des Spiels erstellt, worauf die nächsten Wochen aufbauen werden. Für die Einführung in das Programmieren von Spielen mit SDL habe ich folgendes [Tutorial](https://www.youtube.com/watch?v=QQzAHcojEKg&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx) verwendet. Manche Aspekte habe ich dabei direkt auf das Spiel angepasst. 

## GameLoop 

Um das Spiel zum laufen zu bekommen, muss zuerst ein GameLoop eingerichtet werden. Das ist eine Schleife, die in diesem Fall 60 mal pro Sekunde aufgerufen wird. Wie oft das passiert, ist abhängig von der Bildwiederholungsrate. In diesem Fall wird bekommt die Wiederholrate einen festen Wert, da Bewegungen und andere Berechnungen nicht Zeit- sondern Frameabhängig sind. 

```c++
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
```

## 

## Game Klasse

Die wichtigsten Abläufe spielen sich in der Game Klasse ab. Diese sorgt dafür, dass alle Objekte initialisiert werden und beinhaltet den Entity Manager. 

### Initialisierung des Spiels 

Das Spiel kann mit einer beliebigen Fenstergröße, als Fenster oder im Vollbildmodus initialisiert werden. An dieser Stelle wird auch die Spielwelt und die Spielfigur initialisiert. 

```c++
void Game::init(const char* title, int width, int height, bool fullscreen) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		int flags = 0; 
		if (fullscreen) {
			flags = SDL_WINDOW_FULLSCREEN;
		}
		else {
			flags = SDL_WINDOW_SHOWN;
		}

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width, height, flags);	
		renderer = SDL_CreateRenderer(window, -1, 0);		

		isRunning = true; 		
	
	}
	else {
		isRunning = false; 
	}
}
```

### Verarbeitung von User-Input 

Bei jedem Aufruf wird mit der Methode **SDL_PollEvent()** das aktuell geschehene Event in einem statischen Objekt **event**. Dieses kann von überall abgefragt werden, um auf die Eingaben des Benutzers zu reagieren. Bei einem Drück auf die Escape-Taste, wird das Spiel zur Zeit beendet. 

```c++
void Game::handleEvents() {
	SDL_PollEvent(&Game::event);
	switch (event.type)	{
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}
```

### Kamera

Die Kamera verfolgt die Spielfigur dauerhaft. Sie scrollt die Spielwelt, bis die Kamera einen der Ränder erreicht. Dabei handelt es sich nicht im eine echte Kamera, sondern nur ein Objekt, dass die Entfernung des Spielers vom Startpunkt speichert. Jedes Objekt wird um diese Entfernung verschoben. Dabei wird der Eindruck verschaffen, dass die Kamera dem Spieler folgt. 

```c++
//Initialisierung der Kamera
SDL_Rect Game::camera = { X-Offset, Y-Offset, Breite der Spielwelt, Höhe der Spielwelt};

//Berechnung des Offsets
camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - ((WINDOW_WIDTH - PLAYER_WIDTH) / 2));
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - ((WINDOW_HEIGHT - PLAYER_HEIGHT) / 2));

//Stoppen wenn Rand erreicht wurde
if (camera.x < 0) {
		camera.x = 0; 
	}
	if (camera.y < 0) {
		camera.y = 0;
	}
	if (camera.x > (map->width*TILE_WIDTH) - camera.w) {
		camera.x = (map->width * TILE_WIDTH) - camera.w;
	}
	if (camera.y > (map->height*TILE_HEIGHT) - camera.h) {
		camera.y = (map->height * TILE_HEIGHT) - camera.h;
	}
```

### Berechnung des nächsten Bilds 

```c++
void Game::update() {
	manager.refresh();
	manager.update();	
}
```

### Anzeigen der Objekte

```c++
void Game::render() {	
	SDL_RenderClear(renderer);
	manager.render();
	SDL_RenderPresent(renderer);
}
```

### Beenden des Spiels

```c++
void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
```



## Entity-Component-System 

Die Verwaltung der Objekte im Spiel wird durch ein Entity-Component System realisiert. 

### Entity 

Jedes Objekt im Spiel ist ein Entity. Ein Entity ist ein Objekt, das durch eine Liste von Components definiert wird. Es können jeder Zeit Components hinzugefügt, entfernt oder abgefragt werden. Die **update()** und **render()** Methoden werden an die Components weitergeleitet. 
Jedes Entity kann in einer Gruppe sein. Diese Gruppe definiert, wann das Entity gezeichnet wird. 

```c++
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
```

### Component 

Ein Component ist ein Bestandteil eines Entitys. Jedes Component steht für eine Eigenschaft oder eine Fähigkeit des Entitys. Was die Components machen, wird in den individuellen Components definiert, die von der Klasse **Component** erben. 

```c++
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
```

### Manager

Der Manager speichert Entities und verwaltet diese. Der Manager hat eine Liste aller Entities und eine Liste aller Gruppen und für jede Gruppe die Entities, die dieser Gruppe angehören. Alle zum löschen bereite Entitys werden in der **refresh()** Methode gelöscht. In der **render()** Methode werden alle Entities gezeichnet. Die Reihenfolge wird durch die Reihenfolge der Gruppen im **groupLabel** Enum. 

```c++
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

**groupLabels**

```c++
enum groupLabels {
	groupBackground,
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders,
	groupForeground
};
```



### Components

#### TransformComponent

Das TransformComponent stellt alle Positions und Bewegungs Aspekte dar. Hier wird die aktuelle Position gespeichert und die Beschleunigung, falls sich das Objekt bewegen soll. Falls das Objekt von der Schwerkraft beeinflusst werden soll, wird das im Konstruktor angegeben. Wenn das der Fall ist wird ein Vektor **gravity** auf die neue Position angewendet. 

##### Berechnung der neuen Position

```c++
void TransformComponent::update() {
	position.x += velocity.x * speed;
    position.y += velocity.y * speed;
    if (gravityAffected) {
        position.x += gravity.x;
        position.y += gravity.y;
    }
}
```



#### SpriteComponent 

Das SpriteComponent ist das Aussehen des Entitys. Es wird immer an der Position, die im TransformComponent gespeichert wird, gezeichnet. 

Wenn das Entity animiert werden soll, wird dies beim Erstellen im Konstruktor angeben. Wenn dies der Fall ist kann man beliebig viele Animationen hinzufügen. Welche abgespielt wird, definiert die Variable **animationIndex**. Um die gespielte Animation zu wechseln, wird die Methode **switchAnimation()** verwendet. In der **update()** Methode wird der aktuelle Frame berechnet. 

```c++
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
```

```c++
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
```

```c++
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



##### Animation

```c++
struct Animation {
	int index;
	int frames;
	int speed; 

	Animation() {
		index = NULL;
		frames = NULL; 
		speed = NULL;
	}
	Animation(int i, int f, int s) {
		index = i; 
		frames = f; 
		speed = s; 
	}
};
```



#### KeyboardController

Der KeyboardController verarbeitet alle Tastatureingaben des Benutzers und gibt diese an das TransformComponent. Bei den Tasten A und D wird die Spielfigur nach Rechts und Links bewegt. Dabei wird die Beschleunigung auf der X-Achse auf 1 oder -1 gesetzt. Werden A und D gleichzeitig gedrückt, bewegt sich der Spieler nicht. Außerdem wird die Animation der Spielfigur gewechselt. 

```c++
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



#### ColliderComponent 

Physische Objekte, die mit anderen kollidieren sollen, bekommen ein ColliderComponent. Dies wird durch ein SDL_Rect realisiert, das immer auf die Position des TransformComponents gesetzt wird und die Größe des Entitys hat. 

#### TileComponent

Das TileComponent ist Bestandteil der Spielwelt. Es ist eine Kombination aus einem vereinfachten TransformComponent (nur Position) und einem SpriteComponent. Bei der Anzahl an Tiles in einer Spielwelt, wird dadurch die benötigte Menge an Components reduziert.

## Kollsionsberechnung

Damit die Spielfigur nicht einfach durch die Welt fällt oder durch Wände gehen kann, muss nach jedem Schritt abgefragt werden, ob eine Kollision vorliegt. Falls dies der Fall ist, wird die Position des Spielers korrigiert bis keine Kollision mehr vorhanden ist. 

### Abfrage der Überlappung

```c++
bool Collision::AABB(const SDL_Rect& a, const SDL_Rect& b) {
	if (a.x + a.w >= b.x && b.x + b.w >= a.x && a.y + a.h >= b.y && b.y + b.h >= a.y) {
		return true; 
	}
	return false; 
}
```

### Überprüfung aller Collider und Positionskorrektur

Bei der Kollisionsabfrage werden einmal alle Collider abgefragt, ob sie sich mit dem Collider der Spielfigur überschneiden. Bei einer Überschneidung muss für die Korrektur die Position der Spielfigur zum kollidierenden Objekt berechnet werden. Abhängig von der Position zum Objekt wird die Position der Spielfigur verändert.

```c++
SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	
	for (auto& c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {					
			if (cCol.y < playerCol.y + PLAYER_HEIGHT && playerCol.y < cCol.y + cCol.h) {
				//right 
				if (playerCol.x + playerCol.w - cCol.x >= 0 && playerCol.x + playerCol.w - cCol.x < 20) {
					playerCol.x = cCol.x - PLAYER_WIDTH - 1;	
					if (player.getComponent<StateComponent>().currentState == dashing) {
						player.getComponent<TransformComponent>().stopDash();
					}
				}
				//left 
				if (playerCol.x > cCol.x && playerCol.x < cCol.x + cCol.w && cCol.x + cCol.w - playerCol.x < 20) {
					playerCol.x = cCol.x + cCol.w + 1;				
					if (player.getComponent<StateComponent>().currentState == dashing) {
						player.getComponent<TransformComponent>().stopDash();
					}
				}
			}
			if (cCol.x < playerCol.x + PLAYER_WIDTH && cCol.x + cCol.w > playerCol.x && player.getComponent<StateComponent>().currentState != dashing) {
				//above
				if (playerCol.y + PLAYER_HEIGHT > cCol.y && playerCol.y + PLAYER_HEIGHT < cCol.y + cCol.h && playerCol.y + PLAYER_HEIGHT - cCol.y <= yDist) {
					playerCol.y = cCol.y - PLAYER_HEIGHT - 1;
					player.getComponent<TransformComponent>().stopJump();
				}
				//below 
				if (playerCol.y > cCol.y && playerCol.y < cCol.y + cCol.h) {
					playerCol.y = cCol.y + cCol.h + 1;
					player.getComponent<TransformComponent>().startFall();
				}
			}			
		}
	}
	player.getComponent<TransformComponent>().position.x = playerCol.x;
	player.getComponent<TransformComponent>().position.y = playerCol.y;
```



## Spielwelt

Jedes Level besteht aus ganz vielen einzelnen Tiles. Ein Level wird in zwei Dateien gespeichert. In der ersten Datei wird definiert, wo welches Tile gezeichnet wird. In der zweiten Datei wird definiert, an welchen Stellen, Collider eingefügt werden, damit der Spieler nicht durch die Welt fällt. Das wird separat gemacht, um nicht unnötig viele Collider zu erstellen und um Speicherplatz zu sparen.

### Einlesen eines Levels

In der Datei stehen Zahlen. Diese definieren, welche Tile aus dem Tileset benutzt wird. Bei -1 wird kein Tile erstellt. Jede andere Zahl steht für ein Tile im Tileset. 

```c++
void TileMap::loadMap(std::string path){
	int srcX, srcY;
	
	std::fstream mapFile (path); 
	std::string line;	

	int x = 0;
	int y = 0;

	while (std::getline(mapFile, line)) {
		std::stringstream s(line);
		std::string cell; 
		while (std::getline(s, cell, ',')) {
			if (cell != "-1") {
				srcX = (stoi(cell) % TILESET_WIDTH) * TILE_WIDTH;
				srcY = (stoi(cell) / TILESET_WIDTH) * TILE_HEIGHT;
				addTile(srcX, srcY, x * TILE_WIDTH, y * TILE_HEIGHT);
			}
			x++;
		}
		if (x > width) {
			width = x; 
		}
		x = 0; 
		if (y > height) {
			height = y;
		}
		y++;		
	}
	
	mapFile.close();	
}
```

### Einlesen der Collider

```c++
void TileMap::loadColliders(std::string path) {

	std::fstream mapFile(path);
	std::string line;

	char c; 

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			mapFile.get(c);
			if (c == '1') {				
				addCollider(x, y);
			}
			mapFile.ignore();
		}
	}
	mapFile.close();
}
```



## TextureManager

Der TextureManager ist eine Klasse mit den statischen Methoden **draw()** und **loadTexture()**. Die **draw()** Methode zeichnet eine Textur an einer bestimmten Stelle, mit einer bestimmten Größe. Diese Textur kann gespiegelt werden. Die **loadTexture()** Methode generiert aus einer Bilddatei eine Textur die im Spiel gezeichnet werden kann. 

```c++
SDL_Texture* TextureManager::loadTexture(const char* filename) {
	SDL_Surface* tmpSurface = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return texture;
}

void TextureManager::draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(Game::renderer, texture, &srcRect, &destRect, NULL, NULL, flip);
}
```

