# Woche 7

## Zusammenfassung

Diese Woche wurde der LevelParser komplett neu aufgebaut und es ist nun möglich zwischen mehreren Leveln zu wechseln, sobald sich in einen bestimmten Bereich begeben wird. 

## Level Parser

Der LevelParser ließt das Level nun aus einer einzigen Datei ein und die durch das Programm PyxelEdit exportierte Datei kann nun ohne Modifikationen eingelesen werden. Die Größe der Karte wird direkt am Anfang eingelesen und danach das Level Ebene für Ebene eingelesen.

```c++
void Map::loadMap(std::string level) {
	std::fstream mapFile("assets/levels/" + level +".map");
	std::string line;

	//Get Map Dimensions
	std::getline(mapFile, line);	
	width = stoi(line.substr(line.find(" ") + 1, line.length()));
	std::getline(mapFile, line);
	height = stoi(line.substr(line.find(" ") + 1, line.length()));

	//Skip unused lines
	for (int i = 0; i < 4; i++) {
		std::getline(mapFile, line);
	}	

	for (int layer = 0; layer < 4; layer++) {
		for (int y = 0; y < height; y++) {
			std::getline(mapFile, line);
			std::stringstream s(line);
			std::string cell;
			int x = 0; 
			int srcX, srcY;
			while (std::getline(s, cell, ',')) {
				if (cell != "-1" && x < width) {
					srcX = (stoi(cell) % TILESET_WIDTH) * TILE_WIDTH;
					srcY = (stoi(cell) / TILESET_WIDTH) * TILE_HEIGHT;
					if (layer < 3) {
						addTile(srcX, srcY, x * TILE_WIDTH, y * TILE_HEIGHT, layer);
					}
					else {
						addCollider(x, y);
					}
				}
				x++;
			}			
		}
		std::getline(mapFile, line);
		std::getline(mapFile, line);
	}

	mapFile.close();
	
	mapFile.open("assets/levels/" + level + "_transitions.map");
	while (std::getline(mapFile, line)) {
		std::cout << line << std::endl; 
		std::stringstream s(line);
		std::string cell;
		std::getline(s, cell, ',');
		int x = stoi(cell);
		std::getline(s, cell, ',');
		int y = stoi(cell);
		std::getline(s, cell, ',');
		int w = stoi(cell);
		std::getline(s, cell, ',');
		int h = stoi(cell);
		std::string level;
		std::getline(s, level, ',');
		std::getline(s, cell, ',');
		int newX = stoi(cell);
		std::getline(s, cell, ',');
		int newY = stoi(cell);
		addTransition(x, y, w, h, level, newX, newY);
	}
	mapFile.close();
}
```

Um Ein Level zu wechseln werden alle Entities in den Ebenengruppen gelöscht und danach die neue Karte geladen. 

```c++
void Map::clearMap() {
	manager.clearGroup(groupBackground);
	manager.clearGroup(groupMidground);
	manager.clearGroup(groupForeground);
	manager.clearGroup(groupColliders);
	manager.clearGroup(groupTransitions);
}
```



## TransitionComponent

Das TransitionComponent stellt einen Bereich dar, in dem der Spieler die Szene wechselt, wenn er sich in den Bereich begibt. Der Collider ist der Bereich für den Szenenwechsel. level ist der Name der Datei des neuen Levels. newX und newY sind die Koordinaten in dem neuen Level.

```c++
class TransitionComponent : public Component {
public:
	SDL_Rect collider; 
	std::string level; 
	int newX, newY;

	TransitionComponent(int x, int y, int w, int h, std::string l, int nX, int nY) {
		collider = { x,y,w,h };
		level = l; 
		newX = nX;
		newY = nY; 
	}

};
```

Überprüfung und Szenewechsel:

```c++
for (auto &t : transitions) {
			SDL_Rect tCol = t->getComponent<TransitionComponent>().collider;
			if (Collision::RectRect(tCol, pCol)) {
				transition.fadeIn();
				m->clearMap();
				m->loadMap(t->getComponent<TransitionComponent>().level);
				player.getComponent<TransformComponent>().position.x = t->getComponent<TransitionComponent>().newX;
				player.getComponent<TransformComponent>().position.y = t->getComponent<TransitionComponent>().newY;
				transition.fadeOut();
			}
		}
```

Für die Platzierung der Szenenübergänge wird eine neue Datei eingelesen, in der alle Übergänge für das Level eingetragen sind. 

```c++
mapFile.open("assets/levels/" + level + "_transitions.map");
	while (std::getline(mapFile, line)) {
		std::cout << line << std::endl; 
		std::stringstream s(line);
		std::string cell;
		std::getline(s, cell, ',');
		int x = stoi(cell);
		std::getline(s, cell, ',');
		int y = stoi(cell);
		std::getline(s, cell, ',');
		int w = stoi(cell);
		std::getline(s, cell, ',');
		int h = stoi(cell);
		std::string level;
		std::getline(s, level, ',');
		std::getline(s, cell, ',');
		int newX = stoi(cell);
		std::getline(s, cell, ',');
		int newY = stoi(cell);
		addTransition(x, y, w, h, level, newX, newY);
	}
```

Aussehen der _transition.map:

```
1056,160,32,32,level_2,64,64
```



## Transition

Da das Spiel während des Ladens des neuen Levels stehen bleibt, habe ich einen Übergangseffekt eingebaut. Sobald ein TransitionComponent betreten wird, blendet der Bildschirm langsam zu Schwarz. Sobald die Karte geladen ist, verschwindet der Schwarze Bildschirm langsam. 

```c++
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
```

