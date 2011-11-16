#include "hall.h"
#include "layer.h"
#include "tree.h"
#include <cstdlib>
#include <iostream>

using namespace std;

hall::hall(int newid, int newx, int newy, base_room* origin):base_room(newid, newx, newy, HALL) {
	h_length = 0;
	origin_room = origin;
	inc_x = 0;
	inc_y = 0;
};

bool hall::Expand(tile LayerMap[][HEIGHT]) {
	if(x-1<0 || y-1<0 || x+1>=WIDTH || y+1>=HEIGHT) {
		return false;
	}
	
	doors = 2;
	
	bool hit_a_hall = false;

	if(LayerMap[x-1][y].room == origin_room) {	// HALL NEEDS TO EXPAND +X
		inc_x = 1;
		inc_y = 0;
	}
	else if(LayerMap[x+1][y].room == origin_room) { // HALL NEEDS TO EXPAND -X
		inc_x = -1;
		inc_y = 0;
	}
	else if(LayerMap[x][y-1].room == origin_room) { // HALL NEEDS TO EXPAND +Y
		inc_y = 1;
		inc_x = 0;
	}
	else if(LayerMap[x][y+1].room == origin_room) { // HALL NEEDS TO EXPAND -Y
		inc_y = -1;
		inc_x = 0;
	}
	else {
		return false;
	}
	
	// CHECK CONDITIONS AT FIRST LOCATION
	if(x+inc_y >=0 && x-inc_y>=0 && x+inc_y <WIDTH && x-inc_y<WIDTH &&
		y+inc_x >=0 && y-inc_x>=0 && y+inc_x <HEIGHT && y-inc_x<HEIGHT) {
		if(LayerMap[x+inc_y][y+inc_x].lterrain != WALL || LayerMap[x-inc_y][y-inc_x].lterrain != WALL ||
			LayerMap[x][y].lterrain != WALL) {
			return false;
		}	
	}
	else
		return false;
	
	int hxlength = 0;
	int hylength = 0;
		
	// GROW OUT THE HALL
	while(x+hxlength+inc_x+inc_y >= 0 && x+hxlength+inc_x-inc_y >=0 && x+hxlength+inc_x+inc_y < WIDTH &&
			x+hxlength+inc_x-inc_y < WIDTH && y+hylength+inc_y+inc_x >= 0 && y+hylength+inc_y-inc_x >= 0 &&
			y+hylength+inc_y+inc_x < HEIGHT && y+hylength+inc_y-inc_x < HEIGHT &&
			LayerMap[x+hxlength+inc_x+inc_y][y+hylength+inc_y+inc_x].lterrain == WALL &&
			LayerMap[x+hxlength+inc_x-inc_y][y+hylength+inc_y-inc_x].lterrain == WALL &&
			LayerMap[x+hxlength+inc_x][y+hylength+inc_y].lterrain == WALL) {
		hxlength += inc_x;
		hylength += inc_y;
	}
	
	// CHECK END CONDITIONS
	if(x+hxlength+inc_x <0 || y+hylength+inc_y <0 || x+hxlength+inc_x>=WIDTH || y+hylength+inc_y>=HEIGHT) {
		return false;
	}
	else if(LayerMap[x+hxlength+inc_x][y+hylength+inc_y].lterrain != FLOOR) {
		return false;
	}
	else if(hxlength == 1 || hxlength == -1 || hylength == 1 || hylength == -1) {
		return false;
	}
	
	base_room* newroom;
	if(LayerMap[x+hxlength+inc_x][y+hylength+inc_y].room->type == HALL) {
		hit_a_hall = true;
		newroom = LayerMap[x+hxlength+inc_x][y+hylength+inc_y].room;
	}
	else {
		newroom = this;
	}
	
	// FILL IN THE NEW HALL
	for(int i=inc_x;i*i<hxlength*hxlength;i+=inc_x) {
		LayerMap[x+i][y].lterrain = FLOOR;
		LayerMap[x+i][y].room = newroom;
	}
	for(int i=inc_y;i*i<hylength*hylength;i+=inc_y) {
		LayerMap[x][y+i].lterrain = FLOOR;
		LayerMap[x][y+i].room = newroom;
	}
	
	if(!hit_a_hall) {
		LayerMap[x+hxlength][y+hylength].lterrain = DOOR;
	}
	else {
		LayerMap[x+hxlength][y+hylength].lterrain = FLOOR;
	}
	
	LayerMap[x][y].lterrain = DOOR;
	
	LayerMap[x+hxlength][y+hylength].room = newroom;
	
	LayerMap[x+hxlength+inc_x][y+hylength+inc_y].room->doors++;
	
	// UPDATE HALL VALUES
	end_x = x+hxlength;
	end_y = y+hylength;
	final_room = LayerMap[x+hxlength+inc_x][y+hylength+inc_y].room;
	if(hxlength == 0)
		h_length = hylength;
	else if(hylength == 0)
		h_length = hxlength;
	else
		h_length = 0;
	
	return true;
}

void hall::Place_Doors(tile LayerMap[][HEIGHT], int& nhalls, int nrooms) {
	return;
}


// A player must be able to reach every room on the map from every other room on the map.
// To ensure this, a tree is constructed, its nodes containing the room IDs. 
// Rec_Build_Connections() adds a room and all the rooms connected to it by doors to the tree.
// Rec_Build_Connections() is then called for every room connected to the original room, 
// recursively creating a tree of connections. The tree is then checked in Generate(), 
// and if it does not contain every room, more doors are drawn and the tree is built again.
void hall::Build_Connections(tile LayerMap[][HEIGHT], tree* ConTree, node* parent) {
	// Locate doors on every wall and identify the rooms they lead to
	// If a room is already in the tree, ignore it
	// If a room is not in the tree, add it and then
	// call Build_Connections for it.
	
	int inc_r, first, length;
	if(inc_x == 0 && inc_y == 0) {
		cout << "ERROR: both incs 0" << endl;
		return;
	}
	if(inc_x == 0) {
		inc_r = inc_y;
		first = y;
	}
	else if(inc_y ==0){
		inc_r = inc_x;
		first = x;
	}
	else {
		cout << "ERROR: both incs not zero" << endl;
		return;
	}
	
	// CHECK ENDPOINTS
	if(!ConTree->search_tree(origin_room->ID)) {
		ConTree->add_child(parent, origin_room->ID);
		origin_room->Build_Connections(LayerMap, ConTree, parent->firstchild);
	}
	if(!ConTree->search_tree(final_room->ID)) {
		ConTree->add_child(parent, final_room->ID);
		final_room->Build_Connections(LayerMap, ConTree, parent->firstchild);
	}
	
	int side_hlength;
	int side_x;
	int side_y;
	
	base_room* newroom;
	if(doors > 2) {
		// FIND DOORS
		for(int i=0;i*i<=h_length*h_length;i+=inc_r) {
			if(LayerMap[x+inc_y+(i*inc_x*inc_x)][y+inc_x+(i*inc_y*inc_y)].lterrain == DOOR ||
				LayerMap[x+inc_y+(i*inc_x*inc_x)][y+inc_x+(i*inc_y*inc_y)].lterrain == FLOOR ) {	
				newroom = LayerMap[x+inc_y+inc_y+(i*inc_x*inc_x)][y+inc_x+inc_x+(i*inc_y*inc_y)].room;
				if(newroom == NULL) {
					cout << "ERROR: newroom NULL" << endl;
					return;
				}
				else if(newroom == this) {
					side_hlength = 0;
					side_x = x+inc_y+(i*inc_x*inc_x);
					side_y = y+inc_x+(i*inc_y*inc_y);
					while(LayerMap[side_x+(side_hlength*inc_y)][side_y+(side_hlength*inc_x)].room == this) {
						side_hlength+=inc_y*inc_y;
						side_hlength+=inc_x*inc_x;
					}
					newroom = LayerMap[side_x+(side_hlength*inc_y)][side_y+(side_hlength*inc_x)].room;
				}
				else if(!ConTree->search_tree(newroom->ID)) {
					ConTree->add_child(parent, newroom->ID);
					newroom->Build_Connections(LayerMap, ConTree, parent->firstchild);
				}
			}
			if(LayerMap[x-inc_y+(i*inc_x*inc_x)][y-inc_x+(i*inc_y*inc_y)].lterrain == DOOR ||
				LayerMap[x-inc_y+(i*inc_x*inc_x)][y-inc_x+(i*inc_y*inc_y)].lterrain == FLOOR ) {
				newroom = LayerMap[x-inc_y-inc_y+(i*inc_x*inc_x)][y-inc_x-inc_x+(i*inc_y*inc_y)].room;
				if(newroom == NULL) {
					cout << "ERROR: newroom NULL" << endl;
					return;
				}
				else if(newroom == this) {
					side_hlength = 0;
					side_x = x+inc_y+(i*inc_x*inc_x);
					side_y = y+inc_x+(i*inc_y*inc_y);
					while(LayerMap[side_x+(side_hlength*inc_y)][side_y+(side_hlength*inc_x)].room == this) {
						side_hlength+=inc_y*inc_y;
						side_hlength+=inc_x*inc_x;
					}
					newroom = LayerMap[side_x+(side_hlength*inc_y)][side_y+(side_hlength*inc_x)].room;
				}
				else if(!ConTree->search_tree(newroom->ID)) {
					ConTree->add_child(parent, newroom->ID);
					newroom->Build_Connections(LayerMap, ConTree, parent->firstchild);
				}
			}
		}
	}
}

void hall::Light_Room(tile LayerMap[][HEIGHT]) {
	int inc_r, first, length;
	if(inc_x == 0 && inc_y == 0) {
		cout << "ERROR: both incs 0" << endl;
		return;
	}
	if(inc_x == 0) {
		inc_r = inc_y;
		first = y;
	}
	else if(inc_y ==0){
		inc_r = inc_x;
		first = x;
	}
	else {
		cout << "ERROR: both incs not zero" << endl;
		return;
	}

	int side_hlength;
	int side_x;
	int side_y;
	
	for(int i=0;i*i<=h_length*h_length;i+=inc_r) {
		LayerMap[x+(i*inc_x*inc_x)][y+(i*inc_y*inc_y)].lit = true;
		LayerMap[x+inc_y+(i*inc_x*inc_x)][y+inc_x+(i*inc_y*inc_y)].lit = true;
		LayerMap[x-inc_y+(i*inc_x*inc_x)][y-inc_x+(i*inc_y*inc_y)].lit = true;
		LayerMap[x+(i*inc_x*inc_x)][y+(i*inc_y*inc_y)].explored = true;
		LayerMap[x+inc_y+(i*inc_x*inc_x)][y+inc_x+(i*inc_y*inc_y)].explored = true;
		LayerMap[x-inc_y+(i*inc_x*inc_x)][y-inc_x+(i*inc_y*inc_y)].explored = true;
		if(LayerMap[x+inc_y+(i*inc_x*inc_x)][y+inc_x+(i*inc_y*inc_y)].lterrain == FLOOR ) {
			side_hlength = 0;
			side_x = x+inc_y+(i*inc_x*inc_x);
			side_y = y+inc_x+(i*inc_y*inc_y);
			while(LayerMap[side_x+(side_hlength*inc_y)][side_y+(side_hlength*inc_x)].room == this) {
				side_hlength+=inc_y*inc_y;
				side_hlength+=inc_x*inc_x;
				LayerMap[side_x+inc_x+(side_hlength*inc_y)][side_y+inc_y+(side_hlength*inc_x)].lit = true;
				LayerMap[side_x-inc_x+(side_hlength*inc_y)][side_y-inc_y+(side_hlength*inc_x)].lit = true;
				LayerMap[side_x+(side_hlength*inc_y)][side_y+(side_hlength*inc_x)].lit = true;
				LayerMap[side_x+inc_x+(side_hlength*inc_y)][side_y+inc_y+(side_hlength*inc_x)].explored = true;
				LayerMap[side_x-inc_x+(side_hlength*inc_y)][side_y-inc_y+(side_hlength*inc_x)].explored = true;
				LayerMap[side_x+(side_hlength*inc_y)][side_y+(side_hlength*inc_x)].explored = true;
			}
		}
		if(LayerMap[x-inc_y+(i*inc_x*inc_x)][y-inc_x+(i*inc_y*inc_y)].lterrain == FLOOR ) {
			side_hlength = 0;
			side_x = x-inc_y+(i*inc_x*inc_x);
			side_y = y-inc_x+(i*inc_y*inc_y);
			while(LayerMap[side_x-(side_hlength*inc_y)][side_y-(side_hlength*inc_x)].room == this) {
				side_hlength+=inc_y*inc_y;
				side_hlength+=inc_x*inc_x;
				LayerMap[side_x+inc_x-(side_hlength*inc_y)][side_y+inc_y-(side_hlength*inc_x)].lit = true;
				LayerMap[side_x-inc_x-(side_hlength*inc_y)][side_y-inc_y-(side_hlength*inc_x)].lit = true;
				LayerMap[side_x-(side_hlength*inc_y)][side_y-(side_hlength*inc_x)].lit = true;
				LayerMap[side_x+inc_x-(side_hlength*inc_y)][side_y+inc_y-(side_hlength*inc_x)].explored = true;
				LayerMap[side_x-inc_x-(side_hlength*inc_y)][side_y-inc_y-(side_hlength*inc_x)].explored = true;
				LayerMap[side_x-(side_hlength*inc_y)][side_y-(side_hlength*inc_x)].explored = true;
			}
		}
	}
}

void hall::Darken_Room(tile LayerMap[][HEIGHT]) {
int inc_r, first, length;
	if(inc_x == 0 && inc_y == 0) {
		cout << "ERROR: both incs 0" << endl;
		return;
	}
	if(inc_x == 0) {
		inc_r = inc_y;
		first = y;
	}
	else if(inc_y ==0){
		inc_r = inc_x;
		first = x;
	}
	else {
		cout << "ERROR: both incs not zero" << endl;
		return;
	}

	int side_hlength;
	int side_x;
	int side_y;
	
	for(int i=0;i*i<=h_length*h_length;i+=inc_r) {
		LayerMap[x+(i*inc_x*inc_x)][y+(i*inc_y*inc_y)].lit = false;
		LayerMap[x+inc_y+(i*inc_x*inc_x)][y+inc_x+(i*inc_y*inc_y)].lit = false;
		LayerMap[x-inc_y+(i*inc_x*inc_x)][y-inc_x+(i*inc_y*inc_y)].lit = false;
		if(LayerMap[x+inc_y+(i*inc_x*inc_x)][y+inc_x+(i*inc_y*inc_y)].lterrain == FLOOR ) {
			side_hlength = 0;
			side_x = x+inc_y+(i*inc_x*inc_x);
			side_y = y+inc_x+(i*inc_y*inc_y);
			while(LayerMap[side_x+(side_hlength*inc_y)][side_y+(side_hlength*inc_x)].room == false) {
				side_hlength+=inc_y*inc_y;
				side_hlength+=inc_x*inc_x;
				LayerMap[side_x+inc_x+(side_hlength*inc_y)][side_y+inc_y+(side_hlength*inc_x)].lit = false;
				LayerMap[side_x-inc_x+(side_hlength*inc_y)][side_y-inc_y+(side_hlength*inc_x)].lit = false;
				LayerMap[side_x+(side_hlength*inc_y)][side_y+(side_hlength*inc_x)].lit = false;
			}
		}
		if(LayerMap[x-inc_y+(i*inc_x*inc_x)][y-inc_x+(i*inc_y*inc_y)].lterrain == FLOOR ) {
			side_hlength = 0;
			side_x = x-inc_y+(i*inc_x*inc_x);
			side_y = y-inc_x+(i*inc_y*inc_y);
			while(LayerMap[side_x-(side_hlength*inc_y)][side_y-(side_hlength*inc_x)].room == false) {
				side_hlength+=inc_y*inc_y;
				side_hlength+=inc_x*inc_x;
				LayerMap[side_x+inc_x-(side_hlength*inc_y)][side_y+inc_y-(side_hlength*inc_x)].lit = false;
				LayerMap[side_x-inc_x-(side_hlength*inc_y)][side_y-inc_y-(side_hlength*inc_x)].lit = false;
				LayerMap[side_x-(side_hlength*inc_y)][side_y-(side_hlength*inc_x)].lit = false;
			}
		}
	}
}
