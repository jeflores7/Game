//character.h
#ifndef _CHARACTER
#define _CHARACTER

class character{
public:
	character(int CharHealth, int CharAttack, int CharDefense);
	//virtual void takeTurn() = 0;

	//function to process an attack directed towards the character
    //returns 0 if character is not dead
    //returns 1 if attack killed the character
    //returns 2 if attack did no damage
	int takeDamage(int DamageAmount);

	//getters
    int getMax_health();
    int getCurrent_health();
    int getAttack();
    int getDefense();
    //setters
    int setMax_health(int newMaxHealth);
    int setCurrent_health(int newCurentHealth);
    int setAttack(int newAttack);
    int setDefense(int newDefense);
private:
	int max_health;
	int current_health;
	int attack;
	int defense;
};

#endif
