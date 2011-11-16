#include "layer.h"
#include "base_room.h"
#include "rec_room.h"
#include "tree.h"
#include "player.h"
#include "monster.h"
#include "healthPotion.h"
#include <cstdlib>
#include <iostream>

//-----------------------------------------------
// CONSTRUCTORS/INITILIZATION FUNCTIONS FOR LAYER
//-----------------------------------------------
// When a map is initially created, call this constructor with the arguments
// NULL, NULL, and 0. After the first call, descend takes care of layer creation.
layer::layer(layer* previous, layer* next, int newdepth) {
	for(int i=0;i<WIDTH;i++) {
		for(int j=0;j<HEIGHT;j++) {
			LayerMap[i][j].lchar = NULL;
			LayerMap[i][j].litem = NULL;
			LayerMap[i][j].room = NULL;
			LayerMap[i][j].lit = false;
			LayerMap[i][j].explored = false;
		}
	}
	charlist = NULL;
	itemlist = NULL;
	roomlist = NULL;
	player_x = -1;
	player_y = -1;
	prevlayer = previous;
	nxtlayer = next;
	depth = newdepth;
	nrooms = 0;
	nhalls = 0;
	nchars = 0;
	nitems = 0;
	can_scend = true;
	
	Generate();
}

layer::~layer() {
	DeleteMap(this);
}

// Recursively delete the entire list of layers
void DeleteMap(layer* todelete) {
	if(todelete->nxtlayer != NULL)
		DeleteMap(todelete->nxtlayer);
	if(todelete->prevlayer != NULL)
		DeleteMap(todelete->prevlayer);
	delete [] todelete->charlist;
	delete [] todelete->itemlist;
}

// RUN THIS FUNCTION AT THE END OF EVERY ROUND, SETTING MAP EQUAL TO IT
// ie, map = map->Ascend();
// if the player is standing on an UP_LADDER when the round ends
// move the map up one layer, unless this is the first layer.
layer* layer::AscendOrDescend(character *the_player) {
	if(can_scend && LayerMap[player_x][player_y].lterrain == UP_LADDER) {
		if(prevlayer == NULL) {
			return this;
		}
		else {
			DeleteCharacter(player_x, player_y);
			prevlayer->AddCharacter(the_player, prevlayer->player_x, prevlayer->player_y);
			can_scend = false;
			return prevlayer;
		}
	}
	else if(LayerMap[player_x][player_y].lterrain == DOWN_LADDER) {
		if(nxtlayer == NULL) {
			nxtlayer = new layer(this, NULL, depth+1);
		}
		DeleteCharacter(player_x, player_y);
		nxtlayer->AddCharacter(the_player, nxtlayer->player_x, nxtlayer->player_y);
		return nxtlayer;
	}
	return this;
}

//--------------------------------------------
// FUNCTIONS FOR RUN-TIME FUNCTIONING OF LAYER
//--------------------------------------------

// The function is called every time a character takes a turn.
// If the character chose to move into another character's square, Attack is called
// for the attacking character with a pointer to the defender as an argument.
// If the character ends its turn in a square with an item, IsOnItem is called.
// Otherwise, the character is just deleted and added to a new square.
int layer::MoveCharacter(int pos_x, int pos_y, action command) {
	if(LayerMap[pos_x][pos_y].lchar == NULL) {
		return 0;
	}
	
	int move_x = 0;
	int move_y = 0;
	int ahead_x = 0;
	int ahead_y = 0;
	bool move = true;
	switch(command) {
		case MOVE_DOWN:
			if(pos_x+1<WIDTH) {
				move_x = 1;
				ahead_x = 2;
			}
			break;
		case MOVE_UP:
			if(pos_x-1>=0) {
				move_x = -1;
				ahead_x = -2;
			}
			break;
		case MOVE_RIGHT:
			if(pos_y+1<HEIGHT) {
				move_y = 1;
				ahead_y = 2;
			}
			break;
		case MOVE_LEFT:
			if(pos_y-1>=0) {
				move_y = -1;
				ahead_y = -2;
			}
			break;
		default:
			move = false;
			break;
	}
	
	if(move) {
		if(LayerMap[pos_x+move_x][pos_y+move_y].lterrain != WALL) {
			if(LayerMap[pos_x+move_x][pos_y+move_y].lchar != NULL) {
				// attack the character on the square chosen for movement
				LayerMap[pos_x][pos_y].lchar->Attack(LayerMap[pos_x+move_x][pos_y+move_y].lchar);
				if(!LayerMap[pos_x+move_x][pos_y+move_y].lchar->getIsAlive()) {
					DeleteCharacter(pos_x+move_x, pos_y+move_y);
				}
				move_x = 0;
				move_y = 0;
				ahead_x = 0;
				ahead_y = 0;
			}
			else if(LayerMap[pos_x+move_x][pos_y+move_y].litem != NULL) {
				// move to the square chosen for movement
				// call IsOnItem for the item in that square
				AddCharacter(LayerMap[pos_x][pos_y].lchar, pos_x+move_x, pos_y+move_y);
				DeleteCharacter(pos_x, pos_y);
				if( LayerMap[pos_x+move_x][pos_y+move_y].lchar->IsOnItem(LayerMap[pos_x+move_x][pos_y+move_y].litem)) {
					DeleteItem(pos_x+move_x, pos_y+move_y);
				}
				can_scend = true;
			}
			else {
				// move to the square chosen for movement
				AddCharacter(LayerMap[pos_x][pos_y].lchar, pos_x+move_x, pos_y+move_y);
				DeleteCharacter(pos_x, pos_y);
				can_scend = true;
			}
		}
		else {
			move_x = 0;
			move_y = 0;
		}
		
		if(pos_x == player_x && pos_y == player_y) {
			player_x = pos_x+move_x;
			player_y = pos_y+move_y;
		}
		
		// light up the next area
		if(LayerMap[pos_x][pos_y].lterrain == DOOR) {
			if(LayerMap[pos_x-move_x][pos_y-move_y].room != NULL) {
				LayerMap[pos_x-move_x][pos_y-move_y].room->Darken_Room(LayerMap);
			}
		}
		if(LayerMap[pos_x][pos_y].lterrain == DOOR) {
			if(LayerMap[pos_x+move_x][pos_y+move_y].room != NULL) {
				LayerMap[pos_x+move_x][pos_y+move_y].room->Light_Room(LayerMap);
			}
		}
		if(LayerMap[pos_x+move_x][pos_y+move_y].lterrain == DOOR ) {
			if(LayerMap[pos_x+ahead_x][pos_y+ahead_y].room != NULL) {
				LayerMap[pos_x+ahead_x][pos_y+ahead_y].room->Light_Room(LayerMap);
			}
		}
		
	}
	return 1;
}

void layer::DeleteItem(int item_x, int item_y) {
	LayerMap[item_x][item_y].litem = NULL;
}

void layer::DeleteCharacter(int char_x, int char_y) {
	LayerMap[char_x][char_y].lchar = NULL;
}

void layer::AddItem(item* add_item, int item_x, int item_y) {
	LayerMap[item_x][item_y].litem = add_item;
}

void layer::AddCharacter(character* add_char, int char_x, int char_y) {
	LayerMap[char_x][char_y].lchar = add_char;
	add_char->setX(char_x);
	add_char->setY(char_y);
}

//-----------------------------
// GETTERS/SETTERS FOR LAYERMAP
//-----------------------------
character* layer::Get_Character(int number) {
	return charlist[number];
}

int layer::Get_Nchars() {
	return nchars;
}

int layer::Get_player_x() {
	return player_x;
}

int layer::Get_player_y() {
	return player_y;
}


//-------------------------------------
// FUNCTIONS FOR GENERATION OF LAYERMAP
//-------------------------------------

// This function is called once at layer creation and generates the map. It calls Seed(),
// Rec_Build_Connections(), and PlaceLadders().
void layer::Generate() {
	// GENERATE NUMBER OF ROOMS based on dimensions and ROOM_DENSITY variable
	// this forumla comes from trial and error and can be adjusted whenever.
	nrooms = (WIDTH*HEIGHT*ROOM_DENSITY)/(MAX_ROOM_RADIUS*MAX_ROOM_RADIUS);
	nrooms += rand()%ROOM_DENSITY;
	
	// create an array of room pointers of length NROOMS to hold future rooms
	roomlist = new base_room*[nrooms];
	
	// set all tile rooms to NULL
	for(int x=0;x<WIDTH;x++) {
		for(int y=0;y<HEIGHT;y++) {
			LayerMap[x][y].room = NULL;
			LayerMap[x][y].lterrain = WALL;
		}
	}
	
	// SEED THE MAP
	Seed();
	
	// GROW THE ROOMS
	// cycle through the roomlist until all rooms have finished growing
	int done_yet = 0;
	while(done_yet < nrooms) {
		for(int i=0;i<nrooms;i++) {
			if(roomlist[i]->can_grow && rand()%ROOM_VAR == 0) {
				if(!roomlist[i]->Expand(LayerMap)) {
					done_yet++;
				}
			}
		}
	}
	// PLACE DOORS AND BUILD HALLS
	if(!Build_Doors()) {
		Generate();
		return;
	}
	
	// PLACE LADDERS (and the player at the location of UP_LADDER)
	PlaceLadders();
	
	// LIGHT ROOM CONTAINING THE PLAYER
	LayerMap[player_x][player_y].room->Light_Room(LayerMap);
	
	// PLACE CHARACTERS
	nchars = 1;
	charlist = new character*[nchars];
	// TODO: randomly generate monsters based on depth
	// for now, just make one generic monster.
	for(int i=0;i<nchars;i++) {
		charlist[i] = new monster('m');
		int char_x;
		int char_y;
		do {
			char_x = rand()%WIDTH;
			char_y = rand()%HEIGHT;
		} while(LayerMap[char_x][char_y].lterrain != FLOOR || LayerMap[char_x][char_y].lchar != NULL);
		charlist[i]->setX(char_x);
		charlist[i]->setY(char_y);
		LayerMap[char_x][char_y].lchar = charlist[i];
	}
	
	// PLACE ITEMS
	nitems = 1;
	itemlist = new item*[nitems];
	for(int i=0;i<nitems;i++) {
		itemlist[i] = new healthPotion();
		int item_x;
		int item_y;
		do {
			item_x = rand()%WIDTH;
			item_y = rand()%HEIGHT;
		} while(LayerMap[item_x][item_y].lterrain != FLOOR || LayerMap[item_x][item_y].litem != NULL);
		LayerMap[item_x][item_y].litem = itemlist[i];
	}
	// TODO: randomly generate items based on depth
}

bool layer::Build_Doors() {
	// PLACE DOORS
	// iterate through the roomlist and place doors on rooms
	bool connected;
	int count = 0;
	
	int start_room = 0;
	while(start_room<nrooms && roomlist[start_room]->type == REMOVED)
		start_room++;
	if(start_room == nrooms) {
		return false;
	}
	
	for(int i=0;i<nrooms;i++) {
		roomlist[i]->Place_Doors(LayerMap, nhalls, nrooms);
	}
	do{
		tree* ConTree=new tree(start_room);
		node* first = ConTree->get_first();
		roomlist[start_room]->Build_Connections(LayerMap, ConTree, first);
		connected = true;
		for(int i=0;i<nrooms;i++) {
			if(roomlist[i]->type != REMOVED)
				if(!ConTree->search_tree(i)) {
					connected = false;
					roomlist[i]->Place_Doors(LayerMap, nhalls, nrooms);	
				}
		}
		count++;
		if(count>=MAX_REPEATS) {
			return false;
		}
	}while(!connected);
	return true;
}

// Seed() fills the roomlist array with new rooms, their x and y coordinates
// and type randomly selected. No rooms will be seeded within one square
// of each other in any direction. A room has a 1/3 chance of being a circle
// and a 2/3 chance of being a rectangle. (NOTE: circles removed for now)
void layer::Seed() {
	
	// CYCLE THROUGH LIST OF ROOMS, CREATING SEEDS (X, Y, TYPE)
	for(int i=0;i<nrooms;i++) {
		int x = rand()%(WIDTH-2) +1; // random x coordinate
		int y = rand()%(HEIGHT-2) +1; // random y coordinate 
		
		// check if this location or an near location already has a seed
		// If so, redo seeding.
		if(LayerMap[x][y].room == NULL && 
			LayerMap[x-1][y].room == NULL && 
			LayerMap[x+1][y].room == NULL && 
			LayerMap[x-1][y-1].room == NULL && 
			LayerMap[x][y-1].room == NULL && 
			LayerMap[x+1][y-1].room == NULL &&
		 	LayerMap[x-1][y+1].room == NULL && 
		 	LayerMap[x][y+1].room == NULL && 
		 	LayerMap[x+1][y+1].room == NULL ) {
				LayerMap[x][y].lterrain = FLOOR;
				/*if(rand()%3 == 0) {
					roomlist[i] = new circle_room(i,x,y);
				}
				else {
					roomlist[i] = new rec_room(i,x,y);
				}*/ // CIRCLES REMOVED FOR NOW
				// TODO: Add functionality for circular rooms. Not really essential, but pretty.
				roomlist[i] = new rec_room(i,x,y);
				LayerMap[x][y].room = roomlist[i];
		}
		else
			i--;
	}
}

// Choose a random location for the UP_LADDER and DOWN_LADDER
// For now, the only restriction is that both are not on walls or in the same square.
// TODO: ensure that the distance between the ladders is sufficiently large
void layer::PlaceLadders() {
	int pos_up_x, pos_up_y, pos_down_x, pos_down_y;
	
	do {
		do{
			pos_up_x = rand()%WIDTH;
			pos_up_y = rand()%HEIGHT;
		} while(LayerMap[pos_up_x][pos_up_y].room == NULL);
	} while(LayerMap[pos_up_x][pos_up_y].room->ID >= nrooms || LayerMap[pos_up_x][pos_up_y].room->type == REMOVED);
	
	do {
		do{
			pos_down_x = rand()%WIDTH;
			pos_down_y = rand()%HEIGHT;
		} while(LayerMap[pos_down_x][pos_down_y].room == NULL);
	} while(LayerMap[pos_down_x][pos_down_y].room->ID >=nrooms || 
			  LayerMap[pos_down_x][pos_down_y].room == LayerMap[pos_up_x][pos_up_y].room ||
			  (pos_down_x == pos_up_x && pos_down_y == pos_up_y) );
			
	LayerMap[pos_up_x][pos_up_y].lterrain = UP_LADDER;
	LayerMap[pos_down_x][pos_down_y].lterrain = DOWN_LADDER;
	player_x = pos_up_x;
	player_y = pos_up_y;
}


// PRINT FUNCTION ------------------------
void layer::Print(WINDOW* mapw) {
	wmove(mapw,0,0);
	for(int i=0;i<WIDTH;i++) {
		 wmove(mapw, i,0);
		 for(int j=0;j<HEIGHT;j++) {
		 	if(LayerMap[i][j].lit || LayerMap[i][j].explored) {
		 		if(LayerMap[i][j].explored && !LayerMap[i][j].lit)
		 			wattron(mapw, A_DIM);
				if(LayerMap[i][j].lchar != NULL) {
					if(i==player_x && j==player_y) {
						wattron(mapw, COLOR_PAIR(3));
						wattron(mapw, A_BOLD);
		  				waddch(mapw, '@');
		  				wattroff(mapw, COLOR_PAIR(3));
		  				wattroff(mapw, A_BOLD);
		  			}
					else {
						wattron(mapw, COLOR_PAIR(4));
		  				waddch(mapw, 'm');
						wattroff(mapw, COLOR_PAIR(4));
					}
				}
				else if(LayerMap[i][j].litem != NULL) {
					wattron(mapw, COLOR_PAIR(5));
					waddch(mapw, '!');
					wattroff(mapw, COLOR_PAIR(5));
				}
				else {
					switch(LayerMap[i][j].lterrain) {
						case FLOOR:
							wattron(mapw, COLOR_PAIR(2));
				 			waddch(mapw, '.');
				 			wattroff(mapw, COLOR_PAIR(2));
				  			break;
						case WALL:
							wattron(mapw, COLOR_PAIR(1));
						  	waddch(mapw, '#');
						  	wattroff(mapw, COLOR_PAIR(1));
						  	break;
						case DOOR:	
							wattron(mapw, COLOR_PAIR(6));
						  	waddch(mapw, '+');
						  	wattroff(mapw, COLOR_PAIR(6));
						  	break;
						case UP_LADDER:
							wattron(mapw, COLOR_PAIR(6));
							wattron(mapw, A_BOLD);
						  	waddch(mapw, '<');
						  	wattroff(mapw, A_BOLD);
						  	wattroff(mapw, COLOR_PAIR(6));
						  	break;
						case DOWN_LADDER:
							wattron(mapw, COLOR_PAIR(6));
							wattron(mapw, A_BOLD);
						  	waddch(mapw, '>');
						  	wattroff(mapw, A_BOLD);
						  	wattroff(mapw, COLOR_PAIR(6));
						  	break;
					} // end switch
				} // end top 'if' statement
				if(LayerMap[i][j].explored && !LayerMap[i][j].lit)
					wattroff(mapw, A_DIM);
			}
			else
				waddch(mapw, ' ');
		} // end HEIGHT for-loop
	} // end WIDTH for-loop
} // end print()
