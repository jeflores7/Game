//player.cpp

#ifndef NULL
#define NULL 0

#include "player.h"
#include <iostream>

using namespace std;

	//-------------------
	// player constructor
	//-------------------
	player::player():character(PLAYER_START_HP, PLAYER_START_ATK, PLAYER_START_DEF){
		gold = 0;
		for(int i=0;i<20;i++)
			inventory[i] = NULL;
		equippedArmor = NULL;
		equippedWeapon = NULL;
		current_nitems = 0;
	}
	
	//-----------------
	// player functions
	//-----------------

	char player::TakeTurn() {
		return 'u';
	}

	void player::Attack(character* defender){
		int attack;
		if (equippedWeapon!= NULL)
			attack= this->getAttack()+equippedWeapon->getBonusAttack();
		else
			attack= this->getAttack();
		defender->BeAttacked(attack);
	}
		
	void player::BeAttacked(int attack) {
		int damage;
		int new_health;
		if(equippedArmor != NULL)
			damage = this->getAttack() - (this->getDefense()+equippedArmor->getBonusArmor());
		else
			damage = this->getAttack() - this->getDefense();
		if(damage >0) {
			new_health = this->getCurrent_health() - damage;
			if(new_health <= 0) {
				this->setIsAlive(false);
			}
		}	
		this->setCurrent_health(new_health);	
	}
	
	bool player::IsOnItem(item* on_item) {
		return addToInventory(on_item);
	}

	//adds value to gold
	void player::addGold(int addedValue){
		gold += addedValue;
	}
	
	//adds an item to inventory
	bool player::addToInventory(item* newItem){
		int i=0;
		while(i<MAX_INVENTORY){
			if(inventory[i] == NULL){
				inventory[i]=newItem;
				current_nitems++;
				return true;
			}
			i++;
		}
		return false;
	}

	void player::PrintInventory(WINDOW* inventoryw) {
		wmove(inventoryw, 1, 1);
		wattron(inventoryw, A_BOLD);
		wprintw(inventoryw, "INVENTORY:");
		wattroff(inventoryw, A_BOLD);
		for(int i= 0;i<current_nitems;i++) {
			wmove(inventoryw, 3+i, 1);
			wprintw(inventoryw,"%i. ", i+1);
			waddstr(inventoryw,inventory[i]->getName().c_str());
		}
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

	//---------------
	// player getters
	//---------------
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
