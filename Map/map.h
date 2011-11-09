#include <iostream>
#include <string>
#include <cstdlib>

#include "layer.h"

class map {
public:
	map();
	~map();
	int Ascend();
	int Descend();
	int ReloadLevel(); //reload a previously generated level. Returns 1 if successful, otherwise 0.
	int MoveObject(); //move an object from one square to another. Returns 1 if successful, otherwise 0.
	int DeleteObject(); // delete an object from a square. Returns 1 if successful, otherwise 0.
	int AddObject(); //add an object to an unoccupied square. Returns 1 if successful, otherwise 0.
	int IdentifyObject(); // returns 0 if no object in location, 1 if character, 2 if item
	Get_Monster();
	Get_Item();
	void test();
	int GUIstringArrayLength();
	string *getGUIstrings();
private:
	layer *clayer;
};
