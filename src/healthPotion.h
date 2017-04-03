//healthPotion.h
#ifndef _HEALTH_POTION
#define _HEALTH_POTION

#include "item.h"

#define FIRST_POTION_VALUE 5

class healthPotion:public item{
public:
	// Constructor for healthPotion. Takes no arguments.
	healthPotion();
	// Returns "potionValue", which is the amount of 
	// health the potion will regenerate.
	// (Regeneration not implemented yet)
	int getPotionValue();
private:
	int potionValue;
};

#endif
