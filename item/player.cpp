//player.cpp

#ifndef NULL
#define NULL 0

#include "player.h"
#include <iostream>
using namespace std;

//--------------------//
//-player constructor-//
//--------------------//
player::player(int x, int y):character(100, 10, 0, x, y){
	gold = 0;
	for(int i=0;i<20;i++)
		inventory[i] = NULL;
	equippedArmor = NULL;
	equippedWeapon = NULL;
	}

//------------------//
//-player functions-//
//------------------//

//function that returns the total damage of the character
//returns the sum of attack and weapon bonus
int player::attackValue(){
	if (equippedWeapon!= NULL)
		return this->getAttack()+equippedWeapon->getBonusAttack();
	else
		return this->getAttack();
}
//adds value to gold
void player::addGold(int addedValue){
	gold += addedValue;
}
//adds an item to inventory
//returns 0 if added successfully
//returns 1 if inventory full
int player::addToInventory(item* newItem){
	int i=0;
	while(i<20){
		if(inventory[i] == NULL){
			inventory[i]=newItem;
			return 0;
		}
	}
	return 1;
}
//sets weapon to equip and places equipped weapon to inventory
//returns 0 if equipped successfully
//returns 1 if inventory full
int player::equipArmor(armor *newArmor){
	if(addToInventory(equippedArmor) == 1)
		return 1;
	equippedArmor = newArmor;
	return 0;
}
//sets armor to equip and places equipped armor to inventory
//returns 0 if equipped successfully
//returns 1 if inventory full
int player::equipWeapon(weapon *newWeapon){
	if(addToInventory(equippedWeapon) == 1)
		return 1;
	equippedWeapon = newWeapon;
	return 0;
}

//-------------------//
//---player getters---//
//-------------------//
int player::getGold(){
	return gold;
}
armor* player::getArmor(){
	return equippedArmor;
}
weapon* player::getWeapon(){
	return equippedWeapon;
}
item* player::getInventory(){
	return *inventory;
}

#endif 