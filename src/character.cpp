//character.cpp
#include "character.h"

//--------------------------
// CONSTRUCTORS/DESTRUCTORS
//--------------------------

character::character(int CharHealth, int CharAttack, int CharDefense){
	max_health = CharHealth;
	current_health = CharHealth;
	attack = CharAttack;
	defense = CharDefense;
	alive = true;
	x= -1;
	y = -1;
	}

//-------------------
// CHARACTER GETTERS
//-------------------
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
int character::getX() {
	return x;
}
int character::getY() {
	return y;
}
bool character::getIsAlive() {
	return alive;
}

//-----------------
// CHARACTER SETTERS
//-----------------
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
void character::setX(int newx) {
	x = newx;
}
void character::setY(int newy) {
	y = newy;
}
void character::setIsAlive(bool newstate) {
	alive = newstate;
}
