//armor.cpp
#include "armor.h"

//-------------------//
//-armor constructor-//
//-------------------//
armor::armor(int armorValue):item(false, true){
	bonusArmor = armorValue;
	}

//------------------//
//---armor getter---//
//------------------//
int armor::getBonusArmor(){
	return bonusArmor;
}