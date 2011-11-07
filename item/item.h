//item.h
#ifndef _ITEM
#define _ITEM

class item{
public:
	item(bool usable, bool equipable);
	bool getIsUsable();
	bool getIsEquipable();
private:
	bool isUsable;
	bool isEquipable;
};

#endif