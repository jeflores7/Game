//armor.h
#ifndef _ARMOR
#define _ARMOR

#include "item.h"

class armor:public item{
public:
	// Constructor for armor. Takes as argument a number as the
	// bonus value the armor will apply to a player's defense.
	armor(int armorValue);
	// Getter for the variable "bonusArmor", which is an integer
	// representing the value the armor will apply to a player's defense.
	int getBonusArmor();
private:
	int bonusArmor;
};

#endif
