//character.cpp
#include "character.h"

//-----------------------//
//-character constructor-//
//-----------------------//

character::character(int CharHealth, int CharAttack, int CharDefense){
	max_health = CharHealth;
	current_health = CharHealth;
	attack = CharAttack;
	defense = CharDefense;
	}

//-------------------------//
//--character functions----//
//-------------------------//

//function to process an attack directed towards the character
//returns 0 if character is not dead
//returns 1 if attack killed the character
//returns 2 if attack did no damage
int character::takeDamage(int DamageAmount){
    if ((DamageAmount-defense)<0){
        current_health = current_health;
        return 2;
    }
    else if ((current_health - (DamageAmount-defense))<0){
        current_health = 0;
        return 1;
    }
    else{
        current_health = current_health - (DamageAmount-defense);
        return 0;
    }
}

//---------------------//
//--character getters--//
//---------------------//
int character::getMax_health(){
    return max_health;
}
int character::getCurrent_health(){
    return current_health;
}
int character::getAttack(){
    return attack;
}
int character::getDefense(){
    return defense;
}

//---------------------//
//--character setters--//
//---------------------//
//returns new current amount

//when maxhealth is set under current health, current health
//becomes maxhealth
int character::setMax_health(int newMaxHealth){
    max_health = newMaxHealth;
    if(current_health>max_health)
        current_health = max_health;
    return max_health;
}
//current health cannot be greater than maxhealth
//when user tries to make current health greater than maxhealth
//currenthealth = maxhealth
int character::setCurrent_health(int newCurentHealth){
    if(newCurentHealth>max_health)
        current_health = max_health;
    else
        current_health = newCurentHealth;
    return current_health;
}
int character::setAttack(int newAttack){
    attack = newAttack;
    return attack;
}
int character::setDefense(int newDefense){
    defense = newDefense;
    return defense;
}
