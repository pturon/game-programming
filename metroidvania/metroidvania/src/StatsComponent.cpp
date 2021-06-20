#include "../include/StatsComponent.h"

StatsComponent::StatsComponent(int mH, int mM, int m, int aD, int aS, int iF) {
	maxHealth = mH; 
	curHealth = mH; 
	maxMana = mM;
	curMana = mM;
	money = m; 
	attackDamage = aD; 
	attackSpeed = aS;
	maxIFrames = iF; 
}

void StatsComponent::update() {
	iFrames--;
	if (iFrames < 0) {
		iFrames = 0; 
	}
	if (curHealth == 0) {
		//parent->destroy();
	}
}