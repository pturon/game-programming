#include "../include/EntityComponentSystem.h"

void Entity::addGroup(Group g) {
	groupBitset[g] = true; 
	manager.addToGroup(this, g);
}