#include "rec_room.h"
#include "hall.h"
#include "layer.h"
#include "tree.h"
#include <cstdlib>
#include <iostream>

using namespace std;

//----------------------
// CONSTRUCTORS FOR REC
//----------------------

rec_room::rec_room(int newID, int newx, int newy):base_room(newID, newx, newy, REC) {
	l_radius = 0;
	r_radius = 0;
	t_radius = 0;
	b_radius = 0;
};

//-----------------------
// FUNCTIONS FOR REC_ROOM
//-----------------------

bool rec_room::Expand(tile LayerMap[][HEIGHT]) {
	// A rectangular room expands in every direction.
	// Before expanding in a direction, it checks that there are two
	// tiles of wall in that direction all along that side.
	// If at least one wall expands, then the room can still grow
	// and will return "true". Else it returns "false" and sets the room's
	// "can_grow" variable to false.

	bool has_expanded = false;
	bool do_expand;
	
	if(x+r_radius+2<WIDTH) {
		do_expand = true;
		// EXPAND IN THE +X DIRECTION
		// check that expansion is possible
			for(int i=-t_radius-1;i<=b_radius+1;i++) {
				if(LayerMap[x+r_radius+1][y+i].lterrain != WALL ||
				LayerMap[x+r_radius+2][y+i].lterrain != WALL) {
					do_expand = false;
				}
			}
		// expand
		if(do_expand) {
			for(int i=-t_radius;i<=b_radius;i++) {
				LayerMap[x+r_radius+1][y+i].lterrain = FLOOR;
				LayerMap[x+r_radius+1][y+i].room = this;
			}
			r_radius++;
			has_expanded = true;
		}
	}
	
	if(x-l_radius-2>=0) {
		do_expand = true;
		// EXPAND IN THE -X DIRECTION
		// check that expansion is possible
			for(int i=-t_radius-1;i<=b_radius+1;i++) {
				if(LayerMap[x-l_radius-1][y+i].lterrain != WALL ||
				LayerMap[x-l_radius-2][y+i].lterrain != WALL) {
					do_expand = false;
				}
			}
		// expand
		if(do_expand) {
			for(int i=-t_radius;i<=b_radius;i++) {
				LayerMap[x-l_radius-1][y+i].lterrain = FLOOR;
				LayerMap[x-l_radius-1][y+i].room = this;
			}
			l_radius++;
			has_expanded=true;
		}
	}
	
	if(y+b_radius+2 < HEIGHT) {
		do_expand = true;
		// EXPAND IN THE +Y DIRECTION
		// check that expansion is possible
			for(int i=-l_radius-1;i<=r_radius+1;i++) {
				if(LayerMap[x+i][y+b_radius+1].lterrain != WALL ||
				LayerMap[x+i][y+b_radius+2].lterrain != WALL) {
					do_expand = false;
				}
			}
		// expand
		if(do_expand) {
			for(int i=-l_radius;i<=r_radius;i++) {
				LayerMap[x+i][y+b_radius+1].lterrain = FLOOR;
				LayerMap[x+i][y+b_radius+1].room = this;
			}
			b_radius++;
			has_expanded=true;
		}
	}
	
	if(y-t_radius-2>=0) {
	do_expand = true;
		// EXPAND IN THE -Y DIRECTION
		// check that expansion is possible
			for(int i=-l_radius-1;i<=r_radius+1;i++) {
				if(LayerMap[x+i][y-t_radius-1].lterrain != WALL ||
				LayerMap[x+i][y-t_radius-2].lterrain != WALL) {
					do_expand = false;
				}
			}
		// expand
		if(do_expand) {
			for(int i=-l_radius;i<=r_radius;i++) {
				LayerMap[x+i][y-t_radius-1].lterrain = FLOOR;
				LayerMap[x+i][y-t_radius-1].room = this;
			}
			t_radius++;
			has_expanded=true;
		}
	}
	
	if(!has_expanded) {
		can_grow = false;
		if(l_radius+r_radius <=3 || b_radius+t_radius <=3) {
			type = REMOVED;
			for(int i=-l_radius;i<=r_radius;i++) {
				for(int j=-t_radius;j<=b_radius;j++) {
					LayerMap[x+i][y+j].room = NULL;
					LayerMap[x+i][y+j].lterrain = WALL;
				}
			}
			l_radius = 0;
			r_radius = 0;
			b_radius = 0;
			t_radius = 0;
		}
	}
	
	return has_expanded;
};

void rec_room::Place_Doors(tile LayerMap[WIDTH][HEIGHT], int &nhalls, int nrooms) {
	// Iterate over every wall of the rectangle, randomly choosing door locations
	// The more doors a room has, the less likely it will generate a new one.
	// (The probability of a door generating 1/WALL_LENGTH+(DOORS*DOORS) )
	// When a door generates, a hall is drawn perpendicularly until it reaches
	// another room. A hall needs an area of wall 3 wide to pass through, so that 
	// the hall built will have walls on either side. A hall must hit a room head-one 		
	// 	(not glace it or hit a corner).
	if(type == REMOVED )
		return;
		
	for(int i=-l_radius;i<=r_radius;i++) {
		if(r_radius+l_radius>0 && LayerMap[x+i][y+b_radius+1].room == NULL &&
			rand() % (r_radius+l_radius+(doors*doors)) == 0 ) {
			LayerMap[x+i][y+b_radius+1].room = new hall(nrooms+nhalls, x+i, y+b_radius+1, this);
			if(!LayerMap[x+i][y+b_radius+1].room->Expand(LayerMap) ) {
				delete [] LayerMap[x+i][y+b_radius+1].room;
				LayerMap[x+i][y+b_radius+1].room = NULL;
			}
			else
				nhalls++;
				doors++;
		}
		if(r_radius+l_radius>0 && LayerMap[x+i][y-t_radius-1].room == NULL &&
			rand()%(r_radius+l_radius+(doors*doors)) == 0) {
			LayerMap[x+i][y-t_radius-1].room = new hall(nrooms+nhalls,x+i,y-t_radius-1, this);
			if(!LayerMap[x+i][y-t_radius-1].room->Expand(LayerMap) ) {
				delete [] LayerMap[x+i][y-t_radius-1].room;
				LayerMap[x+i][y-t_radius-1].room = NULL;
			}
			else
				nhalls++;
				doors++;
		}
	}
	for(int i=-t_radius;i<=b_radius;i++) {
		if(t_radius+b_radius>0 && LayerMap[x+r_radius+1][y+i].room == NULL && 
			rand()%(t_radius+b_radius+(doors*doors)) == 0) {
			LayerMap[x+r_radius+1][y+i].room = new hall(nrooms+nhalls, x+r_radius+1, y+i, this);
			if(!LayerMap[x+r_radius+1][y+i].room->Expand(LayerMap) ) {
				delete [] LayerMap[x+r_radius+1][y+i].room;
				LayerMap[x+r_radius+1][y+i].room = NULL;
			}
			else
				nhalls++;
				doors++;
		}
		if(t_radius+b_radius>0 && LayerMap[x-l_radius-1][y+i].room == NULL && 
			rand()%(b_radius+t_radius+(doors*doors)) == 0) {
			LayerMap[x-l_radius-1][y+i].room = new hall(nrooms+nhalls,x+l_radius-1, y+i, this);
			if(!LayerMap[x-l_radius-1][y+i].room->Expand(LayerMap) ) {
				delete [] LayerMap[x-l_radius-1][y+i].room;
				LayerMap[x-l_radius-1][y+i].room = NULL;
			}
			else
				nhalls++;
				doors++;
		}
	}
};


// A player must be able to reach every room on the map from every other room on the map.
// To ensure this, a tree is constructed, its nodes containing the room IDs. 
// Rec_Build_Connections() adds a room and all the rooms connected to it by doors to the tree.
// Rec_Build_Connections() is then called for every room connected to the original room, 
// recursively creating a tree of connections. The tree is then checked in Generate(), 
// and if it does not contain every room, more doors are drawn and the tree is built again.
void rec_room::Build_Connections(tile LayerMap[][HEIGHT], tree* ConTree, node* parent) {
	// Locate doors on every wall and identify the rooms they lead to
	// If a room is already in the tree, ignore it
	// If a room is not in the tree, add it and then
	// call Build_Connections for it.
	
	int newroom;
	// FIND DOORS
	for(int i=-l_radius;i<=r_radius;i++) {
		if(LayerMap[x+i][y+b_radius+1].lterrain == DOOR) {	
			newroom = LayerMap[x+i][y+b_radius+2].room->ID;
			if(!ConTree->search_tree(newroom)) {
				ConTree->add_child(parent, newroom);
				LayerMap[x+i][y+b_radius+2].room->Build_Connections(LayerMap, ConTree, parent->firstchild);
			}
		}
		if(LayerMap[x+i][y-t_radius-1].lterrain == DOOR) {
			newroom = LayerMap[x+i][y-t_radius-2].room->ID;
			if(!ConTree->search_tree(newroom)) {
				ConTree->add_child(parent, newroom);
				LayerMap[x+i][y-t_radius-2].room->Build_Connections(LayerMap, ConTree, parent->firstchild);
			}
		}
	}
	for(int i=-t_radius;i<=b_radius;i++) {
		if(LayerMap[x+r_radius+1][y+i].lterrain == DOOR) {
			newroom = LayerMap[x+r_radius+2][y+i].room->ID;
			if(!ConTree->search_tree(newroom)) {
				ConTree->add_child(parent, newroom);
				LayerMap[x+r_radius+2][y+i].room->Build_Connections(LayerMap, ConTree, parent->firstchild);
			}
		}
		if(LayerMap[x-l_radius-1][y+i].lterrain == DOOR) {
			newroom = LayerMap[x-l_radius-2][y+i].room->ID;
			if(!ConTree->search_tree(newroom)) {
				ConTree->add_child(parent, newroom);
				LayerMap[x-l_radius-2][y+i].room->Build_Connections(LayerMap, ConTree, parent->firstchild);
			}
		}
	}
};

void rec_room::Light_Room(tile LayerMap[WIDTH][HEIGHT]) {
		for(int i=-l_radius-1;i<=r_radius+1;i++) {
			for(int j=-t_radius-1;j<=b_radius+1;j++) {
				LayerMap[x+i][y+j].lit = true;
				LayerMap[x+i][y+j].explored = true;
			}
		}
};

void rec_room::Darken_Room(tile LayerMap[WIDTH][HEIGHT]) {
	for(int i=-l_radius-1;i<=r_radius+1;i++) {
		for(int j=-t_radius-1;j<=b_radius+1;j++) {
			LayerMap[x+i][y+j].lit = false;
		}
	}
};
