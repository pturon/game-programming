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

	void clearGroup(Group g) {
		for (auto &e : groupedEntities[g]) {
			e->destroy();
		}
	}
};



