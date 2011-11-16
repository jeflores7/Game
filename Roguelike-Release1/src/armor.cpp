//armor.cpp
#include "armor.h"

//-------------------------------
// ARMOR CONSTRUCTORS/DESTRUCTORS
//-------------------------------
armor::armor(int armorValue):item(false, true, "Generic armor"){
	bonusArmor = armorValue;
	}

//----------------------
// ARMOR GETTERS/SETTERS
//----------------------
int armor::getBonusArmor(){
	return bonusArmor;
}
