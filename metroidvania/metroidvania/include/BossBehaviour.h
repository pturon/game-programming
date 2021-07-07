#pragma once
#include "BehaviourComponent.h"
#include "EntityComponentSystem.h"
#include "StatsComponent.h"
#include "StateComponent.h"
#include "Constants.h"

class StateComponent;
class StatsComponent; 

class BossBehaviour : public Behaviour{
public: 
	int phase = 1; 

	int rageAttackDuration = 500; 
	int rageAttackCount = 0; 
	int rageAttackMax = 5; 

	int bludgeonRange = 180;
	int slamRange = 64; 
	int slamCloseRange = 64; 

	int slamDuration = 500; 
	int leapRange = 100; 
	int leapCounter = 0; 

	Vector2D target = { 0,0 };

	int dyingDuration = 500;
	int despawnTime = 1000;
	int tickCounter = 0;
	int tickStart = 0;

	int cooldownDuration = 1000; 

	State nextState = idle; 

	StateComponent* state; 
	StatsComponent* stats; 
	TransformComponent* transform; 

	void init() override;
	void update() override;
	void onCollision(Vector2D cn) override;
	void onHit(Direction d) override;
	void die() override;

	void startRage(); 
	void startRageJump();
	void startLeap();
	void startCharge();
	void startSlam();
	void startLeapingBludgeon();
};

