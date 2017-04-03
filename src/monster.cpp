//monster.cpp
#include "monster.h"

//--------------------
// MONSTER CONSTRUCTOR
//--------------------
monster::monster(char type):character(TEST_MONSTER_HP, 0, 0){
	symbol = type;
}
	
	
// ---------------------
//  MONSTER FUNCTIONS
// ---------------------
char monster::TakeTurn() {
	return 'u';
}
	
void monster::Attack(character* defender){
	defender->BeAttacked(this->getAttack());
}
		
void monster::BeAttacked(int attack) {
	attack = attack - this->getDefense();
	int new_health = this->getCurrent_health() - attack;
	if(new_health <= 0)
		this->setIsAlive(false);
	this->setCurrent_health(new_health);
}

bool monster::IsOnItem(item* on_item) {
	return false;
}
	

//------------------------
// MONSTER GETTERS/SETTERS
//------------------------

char monster::getSymbol() {
	return symbol;
}

void monster::setSymbol(char newsymbol) {
	symbol = newsymbol;
}
