//healthPotion.h
#ifndef _HEALTH_POTION
#define _HEALTH_POTION

#include "item.h"

class healthPotion:public item{
public:
	healthPotion(int healthValue);
	int getPotionValue();
private:
	int potionValue;
};

#endif