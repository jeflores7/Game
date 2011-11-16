#ifndef BASE_ROOM_H
#define BASE_ROOM_H


struct tile;
class tree;
struct node;

#define WIDTH 23 // width of map
#define HEIGHT 60 // height of map

enum RoomType {REC, CIRCLE, HALL, REMOVED};

class base_room {
	public:
		// CONSTRUCTOR/DESTRUCTORS
		// constructor for a basic room. Takes an integer for the room ID,
		// two integers for the x and y coordinates of the room seed,
		// and a type for the room (currently only REC is supported)
		base_room(int ID, int x, int y, RoomType);
		// FUNCTIONS
		virtual bool Expand(tile LayerMap[][HEIGHT]) = 0;
		virtual void Place_Doors(tile LayerMap[][HEIGHT], int& nhalls, int nrooms) = 0;
		virtual void Build_Connections(tile LayerMap[][HEIGHT], tree* ConTree, node* parent) = 0;
		virtual void Light_Room(tile LayerMap[][HEIGHT]) = 0;
		virtual void Darken_Room(tile LayerMap[][HEIGHT]) = 0;
		// FRIENDS
		friend class layer;
		// COMMON DATA TO ALL ROOMS
		int ID;
		RoomType type;
		int x;
		int y;
		bool can_grow;
		int doors;
};

#endif
