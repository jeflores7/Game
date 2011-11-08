#ifndef LAYER_H
#define LAYER_H

#include <iostream>
#include <cstdlib>
#include <math.h>
#include "tree.h"


#define WIDTH 23 // width of map
#define HEIGHT 60 // height of map

#define ROOM_DENSITY 10
#define ROOM_VAR 2
#define MAX_ROOM_RADIUS 35
#define ROOM_CONSTRAINT 3
#define DOOR_DENSITY 5

enum TerrainType {FLOOR, WALL, DOOR, LADDER};

class item;
class character;
class monster;

struct tile {
	character *lchar; //pointer to character in this location
	item *litem; //pointer to item in this location
	TerrainType lterrain; //type of the terrain at this location
	int room;
};

class layer{
	public:
		layer();
		~layer();
		void InitMap();
		layer* Ascend();
		layer* Descend();
		int MoveCharacter(int old_x, int old_y, int new_x, int new_y);
		int DeleteItem(int item_x, int item_y);
		int DeleteCharacter(int char_x, int char_y);
		int AddItem(item* add_item, int item_x, int item_y); 
		int AddCharacter(character* add_char, int char_x, int char_y);
		monster* Get_Monster();
		item* Get_Item();
		void Print();
	private:
		tile LayerMap[WIDTH][HEIGHT];
		character *charlist;
		item *itemlist;
		int player_x;
		int player_y;
		int depth;
		layer *prevlayer; //pointer to previous layer. NULL if this is the first layer.
		layer *nxtlayer; //pointer to lower layer. NULL if this is the lowest layer.
		// FUNCTIONS
		void Generate();
		// Helper functions for map generation:
		int ExpandCircle(int x, int y, int i);
		int ExpandRec(int x, int y, int i);
		int PlaceDoors(int x, int y, int i, int t, int nrooms);
		void PlaceLadders();
		void PlaceItems();
		bool CheckConnections(int newnrooms, int x0, int y0);
		void BuildConnections(tree &ConnectTree, node* root, int x0, int y0);
		bool SearchConnections(node* root, int search_for);
};


#endif
