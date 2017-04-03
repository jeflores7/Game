//monster.h
#ifndef _MONSTER
#define _MONSTER

#include "character.h"

#define TEST_MONSTER_HP 5

// This class represents the enemies that the player will fight. In later iterations it will
// be purely virtual and enemies will only come in specific types derived from it.
class monster:public character{
public:
	// Constructor for monsters. It takes one argument, a character,
	// which initilizes the symbol variable and denotes what kind
	// of monster this is.
	monster(char type);
	
	// The monster chooses how to move. The function will be implemented in derived
	// classes to give the monsters AI. It returns a char that functions like input
	// from the keyboard. For now, it returns an INVALID command, so the monster does nothing.
	char TakeTurn();
	
	// The monster attacks the character pointed to by the argument. BeAttacked is called for the argument
	// and given an attack value calculated in this function.
	void Attack(character *defender);
	
	// BeAttacked takes one argument, an int representing the attack against it. It subtracts its 
	// defense from the attack and then subtracts the result from its health. If its health
	// is then below 0, the monster is dead and its is_alive bool is set to false.
	void BeAttacked(int attack);
	
	// This function has one argument, the item occupying the same square as the character
	// Monsters can't interact with items, so this function returns false.
	bool IsOnItem(item* on_item);
	
	// GETTERS AND SETTERS
	char getSymbol();
	void setSymbol(char newsymbol);
private:
	char symbol;
};

#endif
