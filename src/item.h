//item.h
#ifndef _ITEM
#define _ITEM

#include <string>

using namespace std;

class item{
public:
	// Constructor for item objects. Takes three arguments, two bools and a string.
	// The first bool denotes whether or not the item is useable by a player
	// The second bool denotes whether or not the item is equipable by a player
	// The string provides a name for the item, which will be displayed in the player's inventory
	item(bool usable, bool equipable, string type);
	
	// GETTERS AND SETTERS
	// returns bool isUsable
	bool getIsUsable();
	// returns bool isEquipable
	bool getIsEquipable();
	// returns string name
	string getName();
	// sets the string name to newname
	void setName(string newname);
	
private:
	bool isUsable;
	bool isEquipable;
	string name;
};

#endif
