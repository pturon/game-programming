#include "../include/StatsComponent.h"

StatsComponent::StatsComponent(int mH, int mM, int m, int aD, int aS) {
	maxHealth = mH; 
	curHealth = mH; 
	maxMana = mM;
	curMana = mM;
	money = m; 
	attackDamage = aD; 
	attackSpeed = aS;
}
