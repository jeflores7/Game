//monster.h
#ifndef _MONSTER
#define _MONSTER

#include "character.h"

class monster:public character{
public:
	monster(string name);
	string getName();
private:
	string name;
};

#endif