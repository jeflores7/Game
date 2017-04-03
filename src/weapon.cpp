//weapon.cpp
#include "weapon.h"

//--------------------//
//-weapon constructor-//
//--------------------//
weapon::weapon(int weaponValue):item(false, true, "Generic Weapon"){
	bonusAttack = weaponValue;
	}

//-------------------//
//---weapon getter---//
//-------------------//
int weapon::getBonusAttack(){
	return bonusAttack;
}
