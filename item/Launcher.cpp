#include "item.h"
#include "armor.h"
#include "weapon.h"
#include "healthPotion.h"
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

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
	//create test values
	armor testArmor1(5);
	armor testArmor2(15);
	armor testArmor3(40);
	armor testArmor4(60);

	weapon testWeapon1(5);
	weapon testWeapon2(15);
	weapon testWeapon3(40);
	weapon testWeapon4(60);

	healthPotion testHealthPotion1(5);
	healthPotion testHealthPotion2(15);
	healthPotion testHealthPotion3(40);
	healthPotion testHealthPotion4(60);

	//check test values
	testArmorf(testArmor1);
	testArmorf(testArmor2);
	testArmorf(testArmor3);
	testArmorf(testArmor4);

	testWeaponf(testWeapon1);
	testWeaponf(testWeapon2);
	testWeaponf(testWeapon3);
	testWeaponf(testWeapon4);

	testHealthPotionf(testHealthPotion1);
	testHealthPotionf(testHealthPotion2);
	testHealthPotionf(testHealthPotion3);
	testHealthPotionf(testHealthPotion4);

	system("pause>nul");
}
