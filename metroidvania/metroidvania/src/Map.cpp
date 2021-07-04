#include "../include/Map.h"
#include "../include/Game.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

extern Manager manager;

Map::~Map(){}

void Map::clearMap() {
	manager.clearGroup(groupBackground);
	manager.clearGroup(groupMidground);
	manager.clearGroup(groupForeground);
	manager.clearGroup(groupColliders);
	manager.clearGroup(groupTransitions);
	manager.clearGroup(groupEnemies);
}

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

	for (int layer = 0; layer < 5; layer++) {
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
					else if(layer == 3){
						addCollider(x, y);
					}
					else {
						if (cell == "33") {
							addRat(x* TILE_WIDTH, y*TILE_HEIGHT);
						}
						else if (cell == "34") {
							addSkeleton(x*TILE_WIDTH, y*TILE_HEIGHT);
						}
						else if (cell == "35") {
							addBoss(x* TILE_WIDTH, y*TILE_HEIGHT);
						}
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
 
void Map::addTile(int srcX, int srcY, int x, int y, int layer) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, x, y, tileSetPath);
	switch (layer)	{
	case 0:
		tile.addGroup(groupBackground);
		break;
	case 1:
		tile.addGroup(groupMidground);
		break;
	case 2:
		tile.addGroup(groupForeground);
		break;
	default:
		break;
	}	
}

void Map::addCollider(int x, int y) {
	auto& collider(manager.addEntity());
	collider.addComponent<ColliderComponent>("terrain", x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
	collider.addGroup(groupColliders);
}

void Map::addTransition(int x, int y, int w, int h, std::string l, int nX, int nY) {
	auto& transition(manager.addEntity());
	transition.addComponent<TransitionComponent>(x, y, w, h, l, nX, nY);
	transition.addGroup(groupTransitions);
}

void Map::addRat(int x, int y) {	
	auto& enemy(manager.addEntity());
	enemy.addComponent<BehaviourComponent>();
	enemy.addComponent<StateComponent>();
	enemy.addComponent<TransformComponent>(x, y, 48, 16, 1, 2, true);
	enemy.addComponent<ColliderComponent>("Enemy", 0, 0);
	enemy.addComponent<SpriteComponent>("assets/rat_spritesheet.png", true);	
	enemy.getComponent<BehaviourComponent>().setBehaviour<GoombaBehaviour>();
	enemy.addComponent<StatsComponent>(5, 0, 0, 1, 0, 60);
	enemy.addGroup(groupEnemies);
	enemy.getComponent<SpriteComponent>().addAnimation(idle, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(walking, 0, 4, 100);
	enemy.getComponent<SpriteComponent>().switchAnimation(walking);
	enemy.getComponent<SpriteComponent>().addAnimation(dying, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(dead, 0, 1, 100);
}

void Map::addSkeleton(int x, int y) {
	y -= TILE_HEIGHT;
	auto& enemy(manager.addEntity());
	enemy.addComponent<StateComponent>();
	enemy.addComponent<TransformComponent>(x, y, 84, 64, 1, 2, true);
	enemy.addComponent<ColliderComponent>("Enemy", 24, 0);
	enemy.addComponent<SpriteComponent>("assets/skeleton_spritesheet.png", true);
	enemy.addComponent<BehaviourComponent>();
	enemy.getComponent<BehaviourComponent>().setBehaviour<SkeletonBehaviour>();
	enemy.addComponent<StatsComponent>(5, 0, 0, 1, 0, 60);
	enemy.addComponent<AttackComponent>();
	enemy.addGroup(groupEnemies);
	enemy.getComponent<SpriteComponent>().addAnimation(idle, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(walking, 1, 6, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(attackingSide, 2, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(attackCooldown, 4, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(charge, 3, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(dying, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(dead, 0, 1, 100);
	enemy.getComponent<StateComponent>().setState(walking);
}

void Map::addBoss(int x, int y) {
	y -= TILE_HEIGHT;
	auto& enemy(manager.addEntity());
	enemy.addComponent<BehaviourComponent>();
	enemy.addComponent<StateComponent>();
	enemy.addComponent<StatsComponent>(9, 0, 0, 2, 0, 60); 
	enemy.addComponent<TransformComponent>(x, 64, 280, 200, 1, 2, true);
	enemy.addComponent<ColliderComponent>("Enemy", 88, 0);
	enemy.addComponent<SpriteComponent>("assets/boss_spritesheet.png", false);	
	enemy.addComponent<AttackComponent>();	
	enemy.getComponent<BehaviourComponent>().setBehaviour<BossBehaviour>();
	enemy.addGroup(groupEnemies);	
	enemy.getComponent<SpriteComponent>().addAnimation(idle, 0, 1, 100);	
	enemy.getComponent<SpriteComponent>().addAnimation(charge, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(dying, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(dead, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(rageJump, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(rage, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(leaping, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(slam, 0, 1, 100);
	enemy.getComponent<SpriteComponent>().addAnimation(leapingBludgeon, 0, 1, 100);
	enemy.getComponent<StateComponent>().setState(idle);
	//std::cout << "boss" << std::endl; 
}