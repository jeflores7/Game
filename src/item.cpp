//item.cpp
#include "item.h"

//------------------
// ITEM CONSTRUCTOR
//------------------
item::item(bool usable, bool equipable, string type){
	isUsable = usable;
	isEquipable = equipable;
	name = type;
	}

//----------------
// GETTERS/SETTERS
//----------------
bool item::getIsUsable(){
	return isUsable;
}

bool item::getIsEquipable(){
	return isEquipable;
}

string item::getName() {
	return name;
}

void item::setName(string newname) {
	name = newname;
}
