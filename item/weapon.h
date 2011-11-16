//weapon.h
#ifndef _WEAPON
#define _WEAPON

#include "item.h"

class weapon:public item{
public:
	weapon(int weaponValue);
	int getBonusAttack();
private:
	int bonusAttack;
};

#endif