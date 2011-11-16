//item.cpp
#include "item.h"

//------------------//
//-item constructor-//
//------------------//
item::item(bool usable, bool equipable){
	isUsable = usable;
	isEquipable = equipable;
	}

//----------------//
//--item getters--//
//----------------//
bool item::getIsUsable(){
	return isUsable;
}

bool item::getIsEquipable(){
	return isEquipable;
}
