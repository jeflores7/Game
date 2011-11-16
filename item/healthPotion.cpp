//healthPotion.cpp
#include "healthPotion.h"

//--------------------------//
//-healthPotion constructor-//
//--------------------------//
healthPotion::healthPotion(int healthValue):item(true, false){
	potionValue = healthValue;
	}

//-------------------------//
//---healthPotion getter---//
//-------------------------//
int healthPotion::getPotionValue(){
	return potionValue;
}