//weapon.h
#ifndef _WEAPON
#define _WEAPON

#include "item.h"

// This class is for objects of a generic weapon type. It will be virtual in later iterations
// when specific types of weapons are created.
class weapon:public item{
public:
	// Constructor for a generic weapon object. Takes an int as an argument
	// which represents the bonus to a player's attack the weapon will provide
	// when equipped. Calls the default item constructor with the arguments
	// "false, true, "Generic Weapon", meaning a weapon is not useable, is equipable,
	// and would appear in the inventory as "Generic Weapon".
	weapon(int weaponValue);
	
	// Getter that returns the bonus to a player's attack when the weapon is equipped
	int getBonusAttack();
private:
	int bonusAttack;
};

#endif
