#include "../include/SkeletonBehaviour.h"

void SkeletonBehaviour::init() {
	parent->getComponent<TransformComponent>().velocity = { 1,0 };
}

void SkeletonBehaviour::update() {
	if (parent->getComponent<StateComponent>().currentState == charge) {
		tickCounter--;
		if (tickCounter <= 0) {
			tickCounter = attackDuration;
			parent->getComponent<StateComponent>().setState(attackingSide);		
		}
	}
	else if (parent->getComponent<StateComponent>().currentState == attackCooldown) {
		tickCounter--;
		if (tickCounter <= 0) {
			parent->getComponent<StateComponent>().setState(walking);			
		}
	}
	else if (parent->getComponent<StateComponent>().currentState == attackingSide) {
		tickCounter--;
		if (tickCounter <= 0) {
			tickCounter = cooldown;
			parent->getComponent<StateComponent>().setState(attackCooldown);			
		}
	}
	else {
		if (parent->getComponent<TransformComponent>().recoil == false) {
			Vector2D pos = { (parent->getComponent<TransformComponent>().position.x + parent->getComponent<TransformComponent>().width) / 2,  (parent->getComponent<TransformComponent>().position.y + parent->getComponent<TransformComponent>().height) / 2 };
			Vector2D playerPos = { (Game::player.getComponent<TransformComponent>().position.x + Game::player.getComponent<TransformComponent>().width) / 2,  (Game::player.getComponent<TransformComponent>().position.y + Game::player.getComponent<TransformComponent>().height) / 2 };;
			Vector2D dist = { playerPos.x - pos.x, playerPos.y - pos.y };
			int d = sqrt(dist.x * dist.x + dist.y * dist.y);
			if (!Game::rayHitsCollider(pos, dist)) {
				if (d <= attackRange) {
					parent->getComponent<TransformComponent>().velocity = { 0,0 };
					tickCounter = attackCharge;
					parent->getComponent<StateComponent>().setState(charge);			
				}
				else if (d <= detectionRange) {
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
				if (parent->getComponent<TransformComponent>().velocity.x == 0) {
					if (parent->getComponent<SpriteComponent>().spriteFlip) {
						parent->getComponent<TransformComponent>().velocity = { -1,0 };
					}
					else {
						parent->getComponent<TransformComponent>().velocity = { 1,0 };
					}
				}
			}
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