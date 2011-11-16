//healthPotion.cpp
#include "healthPotion.h"

	//--------------------------
	// healthPotion constructor
	//--------------------------
	healthPotion::healthPotion():item(true, false, "Health Potion"){
		potionValue = FIRST_POTION_VALUE;
	}

	//---------------------
	// healthPotion getter
	//---------------------
	int healthPotion::getPotionValue(){
		return potionValue;
	}
