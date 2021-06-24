#include "../include/SkeletonBehaviour.h"

void SkeletonBehaviour::init() {
	parent->getComponent<TransformComponent>().velocity = { 1,0 };
}

void SkeletonBehaviour::update() {
	if (parent->getComponent<TransformComponent>().recoil == false) {
		Vector2D pos = parent->getComponent<TransformComponent>().position;
		Vector2D playerPos = Game::player.getComponent<TransformComponent>().position;
		Vector2D dist = { playerPos.x - pos.x, playerPos.y - pos.y};		
		int d = sqrt(dist.x * dist.x + dist.y * dist.y);
		if (!Game::rayHitsCollider(pos, dist)) {
			if (d <= detectionRange) {
				if (dist.x > 0) {
					parent->getComponent<TransformComponent>().velocity = { 1,0 };
					parent->getComponent<SpriteComponent>().flipAnimation(false);
					parent->getComponent<StateComponent>().currentState == detecting;
				}
				else if (dist.x < 0) {
					parent->getComponent<TransformComponent>().velocity = { -1,0 };
					parent->getComponent<SpriteComponent>().flipAnimation(true);
					parent->getComponent<StateComponent>().currentState == detecting;
				}
			}
			else {
				parent->getComponent<StateComponent>().currentState == walking;
			}
		}
		else {
			parent->getComponent<StateComponent>().currentState == walking;
		}
	}	
}

void SkeletonBehaviour::onCollision(Vector2D cn) {
	if (parent->getComponent<StateComponent>().currentState == walking) {
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
}

void SkeletonBehaviour::onHit(Direction d) {
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
	std::cout << "schmutz digga meddl" << std::endl; 
	parent->getComponent<TransformComponent>().startRecoil(recoilV);
}