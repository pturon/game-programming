#include "../include/GoombaBehaviour.h"

void GoombaBehaviour::init() {
	parent->getComponent<TransformComponent>().velocity = { 1,0 };
}

void GoombaBehaviour::update() {	

}

void GoombaBehaviour::onCollision(Vector2D cn) {
	if (cn.x != 0 && cn.y == 0) {
		parent->getComponent<TransformComponent>().velocity = cn;
		if (cn.x == 1) {
			parent->getComponent<SpriteComponent>().flipAnimation(false);
		}
		else {
			parent->getComponent<SpriteComponent>().flipAnimation(true);
		}		
	}
}

void GoombaBehaviour::onHit(Direction d) {
	Vector2D recoilV = { 0,0 };
	switch (d) {
	case up:
		recoilV.y = -1;
		break;
	case right:
		recoilV.x = 1; 
		break; 
	case down:
		recoilV.y = 1; 
		break; 
	case left: 
		recoilV.x = -1; 
		break;
	default:
		break; 
	}
	parent->getComponent<TransformComponent>().startRecoil(recoilV);
}