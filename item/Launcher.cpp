#include "character.h"
#include "player.h"
#include "item.h"
#include "armor.h"
#include "weapon.h"
#include "healthPotion.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
using namespace std;

int testPlayer(player testPlayer){
	armor *testArmor1 = new armor(5);
	weapon *testWeapon1 = new weapon(5);
	cout << "Testing Player: " << endl;
	cout << "equippedArmor: " << testPlayer.getArmor() << endl;
	cout << "equippedweapon: " << testPlayer.getWeapon() << endl;
	cout << "gold: " << testPlayer.getGold() << endl;
	cout << "Adding 10 gold: " << endl;
	testPlayer.addGold(10);
	cout << "gold: " << testPlayer.getGold() << endl;
	cout << "x: " << testPlayer.getXCoordinate() << endl;
	cout << "y: " << testPlayer.getYCoordinate() << endl;
	cout << "Total attack Value: " << testPlayer.attackValue() << endl;
	cout << "equipping items: " << endl;
	testPlayer.equipArmor(testArmor1);
	testPlayer.equipWeapon(testWeapon1);
	cout << "Total attack Value: " << testPlayer.attackValue() << endl;
	return 0;
}

int testArmorf(armor testValue){
	cout << "Testing Armor" << endl;
	cout << "Bonus Armor: ";
	cout << testValue.getBonusArmor() << endl;
	cout << "Equipable: ";
	cout << testValue.getIsEquipable() << endl;
	cout << "Usable: ";
	cout << testValue.getIsUsable() << endl;
	cout << endl;
	return 0;
}

int testWeaponf(weapon testValue){
	cout << "Testing Weapon" << endl;
	cout << "Bonus Attack: ";
	cout << testValue.getBonusAttack() << endl;
	cout << "Equipable: ";
	cout << testValue.getIsEquipable() << endl;
	cout << "Usable: ";
	cout << testValue.getIsUsable() << endl;
	cout << endl;
	return 0;
}

int testHealthPotionf(healthPotion testValue){
	cout << "Testing Health Potion" << endl;
	cout << "Bonus Health: ";
	cout << testValue.getPotionValue() << endl;
	cout << "Equipable: ";
	cout << testValue.getIsEquipable() << endl;
	cout << "Usable: ";
	cout << testValue.getIsUsable() << endl;
	cout << endl;
	return 0;
}


int main(){
	player mainPlayer(1,2);
	testPlayer(mainPlayer);

	////create test values
	//cout << "something";
	//{armor testArmor1(5);
	//armor testArmor2(15);
	//armor testArmor3(40);
	//armor testArmor4(60);

	//weapon testWeapon1(5);
	//weapon testWeapon2(15);
	//weapon testWeapon3(40);
	//weapon testWeapon4(60);

	//healthPotion testHealthPotion1(5);
	//healthPotion testHealthPotion2(15);
	//healthPotion testHealthPotion3(40);
	//healthPotion testHealthPotion4(60);

	////check test values
	//testArmorf(testArmor1);
	//testArmorf(testArmor2);
	//testArmorf(testArmor3);
	//testArmorf(testArmor4);

	//testWeaponf(testWeapon1);
	//testWeaponf(testWeapon2);
	//testWeaponf(testWeapon3);
	//testWeaponf(testWeapon4);

	//testHealthPotionf(testHealthPotion1);
	//testHealthPotionf(testHealthPotion2);
	//testHealthPotionf(testHealthPotion3);
	//testHealthPotionf(testHealthPotion4);}

 //   cout << endl << "Testing Character: " << endl;

 //   character testchar(100,10,5,3,4);
 //   cout << "Max Health: " << testchar.getMax_health() << endl;
 //   cout << "Current Health: " << testchar.getCurrent_health() << endl;
 //   cout << "Attack: " << testchar.getAttack() << endl;
 //   cout << "Defense: "<< testchar.getDefense() << endl;
 //   cout << "x: "<< testchar.getXCoordinate() << endl;
 //   cout << "y: "<< testchar.getYCoordinate() << endl;


 //   testchar.setMax_health(95);
 //   testchar.setCurrent_health(90);
 //   testchar.setAttack(7);
 //   testchar.setDefense(2);
 //   testchar.setCoordinates(7,12);

 //   cout << "Max Health: " << testchar.getMax_health() << endl;
 //   cout << "Current Health: " << testchar.getCurrent_health() << endl;
 //   cout << "Attack: " << testchar.getAttack() << endl;
 //   cout << "Defense: "<< testchar.getDefense() << endl;
 //   cout << "x: "<< testchar.getXCoordinate() << endl;
 //   cout << "y: "<< testchar.getYCoordinate() << endl;

 //   testchar.takeDamage(6);
 //   cout << testchar.getCurrent_health() << "should be" << " 99" << endl;
 //   testchar.takeDamage(4);
 //   cout << testchar.getCurrent_health() << "should be" << " 99" << endl;
 //   testchar.takeDamage(106);
 //   cout << testchar.getCurrent_health() << "should be" << " 0" << endl;

	system("pause>nul");
}
