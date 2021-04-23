#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

//Forward declaration of classes
class Component;
class Entity;

//Define ComponentID type as size_t
using ComponentID = std::size_t;

//Returns smallest unused ID for new Component Types
inline ComponentID getComponentTypeID() {
	static ComponentID lastID = 0;
	return lastID++;
}

//Returns ID of on existing Component Type
template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

//Maximum amount of Components that Entity can hold
constexpr std::size_t maxComponents = 32;
//Defines variable that indicates if Entity stores specific Component
using ComponentBitSet = std::bitset <maxComponents>;
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
public: 
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
		return componentBitSet[getComponentTypeID<T>];
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

};

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;
public:
	//Updates all stored Entities 
	void update() {
		for (auto& e : entities) e->update();
	}
	//Renders all stotred Entites 
	void render() {
		for (auto& e : entities) e->render();
	}
	//Remove all inactive Entities
	void refresh() {
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity>& mEntity) {
			return !mEntity->isActive();
			}), std::end(entities));
	}
	//Adds Entity to Manager
	Entity& addEntity() {
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};



