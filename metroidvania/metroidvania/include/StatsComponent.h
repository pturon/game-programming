#pragma once
#include "EntityComponentSystem.h"

class StatsComponent : public Component{
public: 
	int maxHealth = 0; 
	int curHealth = 0; 
	int maxMana = 0; 
	int curMana = 0; 
	int money = 0; 
	int souls = 0;
	int iFrames = 0; 
	int maxIFrames = 60; 

	int attackDamage = 0; 
	int attackSpeed = 0; 

	StatsComponent(int mH, int mM, int money, int aD, int aS, int iF);

	void update();
};

