//armor.h
#ifndef _ARMOR
#define _ARMOR

#include "item.h"

class armor:public item{
public:
	armor(int armorValue);
	int getBonusArmor();
private:
	int bonusArmor;
};

#endif