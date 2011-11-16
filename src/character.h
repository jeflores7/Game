//character.h
#ifndef _CHARACTER
#define _CHARACTER

class item;

class character{
public:
	// Constructor for the character class. Takes as argument three integers
	// which represent (in order) a character's total health, attack value, and defense.
	character(int CharHealth, int CharAttack, int CharDefense);
	
	// VIRTUAL FUNCTIONS IMPLEMENTED IN DERIVED CLASSES
	virtual char TakeTurn() = 0;
	virtual void Attack(character* defender) = 0;
	virtual void BeAttacked(int attack) = 0;
	virtual bool IsOnItem(item* on_item) = 0;

	// GETTERS
    int getMax_health();
    int getCurrent_health();
    int getAttack();
    int getDefense();
    int getX();
    int getY();
    bool getIsAlive();
    
    // SETTERS
    int setMax_health(int newMaxHealth);
    int setCurrent_health(int newCurentHealth);
    int setAttack(int newAttack);
    int setDefense(int newDefense);
    void setX(int);
    void setY(int);
    void setIsAlive(bool);
    
private:
	bool alive;
	int max_health;
	int current_health;
	int attack;
	int defense;
	int x;
	int y;
};

#endif
