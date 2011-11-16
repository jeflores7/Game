#ifndef REC_ROOM_H
#define REC_ROOM_H
#include "base_room.h"

#define WIDTH 23 // width of map
#define HEIGHT 60 // height of map

struct tile;
class tree;

class rec_room : public base_room {
	public:
		// CONSTRUCTORS/DESTRUCTORS
		// constructor for a rectangular room. It takes three arguments.
		// The first is an integer representing the room's ID.
		// The second and third are the integer coordinates of the room's seed.
		// It also calls the base room constructor, initilizing it with the seed location
		// and its type, REC.
		rec_room(int ID, int x, int y);
		
		// A rectangular room expands in every direction.
		// Before expanding in a direction, it checks that there are two
		// tiles of wall in that direction all along that side. Then it expands.
		// It updates the room ID of the tiles it expanded into to its ID and
		// changes their lterrain variable to FLOOR.
		// If at least one wall expanded, then it's possible that the room can still grow
		// and the function will return "true". Else it returns "false" and sets the room's
		// "can_grow" variable to false.
		bool Expand(tile LayerMap[][HEIGHT]);
		
		// Iterate over every wall of the rectangle, randomly choosing door locations.
		// The more doors a room has, the less likely it will generate a new one.
		// 		(The probability of a door generating is 1/WALL_LENGTH+(DOORS*DOORS) )
		// When a door generates, a hall is drawn perpendicularly until it reaches
		// another room. A hall needs an area of wall 3 wide to pass through, so that 
		// the hall built will have walls on either side. A hall must hit a room head-one 		
		// 		(not glace it or hit a corner).
		// The room IDs of tiles chosen as halls are set to -1.
		// The lterrain types of the first and last tile are set to DOOR, the rest of the
		// hall is set to FLOOR.
		void Place_Doors(tile LayerMap[WIDTH][HEIGHT], int& nhalls, int nrooms);
		
		void Build_Connections(tile LayerMap[][HEIGHT], tree* ConTree, node* parent);
		
		void Light_Room(tile LayerMap[WIDTH][HEIGHT]);
		
		void Darken_Room(tile LayerMap[WIDTH][HEIGHT]);
		
	private:
		int l_radius;
		int r_radius;
		int t_radius;
		int b_radius;
};

#endif
