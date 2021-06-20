#include "../include/GoombaBehaviour.h"

void GoombaBehaviour::init() {
	parent->getComponent<TransformComponent>().velocity = { 1,0 };
}

void GoombaBehaviour::update() {	
	directionSwitched = false; 
}

void GoombaBehaviour::onCollision(Vector2D cn) {
	if (cn.x != 0 && cn.y == 0) {
		parent->getComponent<TransformComponent>().velocity = cn;
	}
}

void GoombaBehaviour::onHit() {

}