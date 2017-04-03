#include "base_room.h"
#include "layer.h"
#include <cstdlib>

using namespace std;

//----------------------
// CONSTRUCTORS FOR BASE
//----------------------
base_room::base_room(int newID, int newx, int newy, RoomType newtype) {
	ID = newID;
	x = newx;
	y = newy;
	type = newtype;
	doors = 0;
	can_grow = true;
};
