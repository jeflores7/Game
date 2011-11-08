//player.h
#ifndef _PLAYER
#define _PLAYER

#include "character.h"
#include "item.h"
#include "armor.h"
#include "weapon.h"

class player:public character{
public:
	//constructor
	player(int x, int y);

	//function that returns the total damage of the character
	//returns the sum of attack and weapon bonus
	int attackValue();
	//adds value to gold
	void addGold(int addedValue);
	//adds an item to inventory
	int addToInventory(item* newItem);
	//sets weapon to equip and places equipped weapon to inventory
	int equipArmor(armor *newArmor);
	//sets armor to equip and places equipped armor to inventory
	int equipWeapon(weapon *newWeapon);

	//getters
	int getGold();
	armor *getArmor();
	weapon *getWeapon();
	item *getInventory();
private:
	int gold;
	item *inventory[20];
	armor *equippedArmor;
	weapon *equippedWeapon;
};

#endif