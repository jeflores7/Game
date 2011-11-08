#include "layer.h"
#include "item.h"
#include "character.h"

using namespace std;

layer::layer() {
	charlist = NULL;
	itemlist = NULL;
	Generate();
}

void layer::InitMap() {
	prevlayer = NULL;
	nxtlayer = NULL;
	depth = 0;
}

layer::~layer() {
	
}

layer* layer::Ascend() {
	if(prevlayer == NULL)
		return this;
	else	
		return prevlayer;
}

layer* layer::Descend() {
	if(nxtlayer == NULL) {
		nxtlayer = new layer();
		nxtlayer->prevlayer = this;
		nxtlayer->nxtlayer = NULL;
		nxtlayer->depth = depth+1;
	}
	return nxtlayer;
}

int layer::MoveCharacter(int old_x, int old_y, int new_x, int new_y) {
	LayerMap[new_x][new_y].lchar = LayerMap[old_x][old_y].lchar;
	LayerMap[old_x][old_y].lchar = NULL;
}

int layer::DeleteItem(int item_x, int item_y) {
	LayerMap[item_x][item_y].litem = NULL;
}

int layer::DeleteCharacter(int char_x, int char_y) {
	LayerMap[char_x][char_y].litem = NULL;
}
int layer::AddItem(item* add_item, int item_x, int item_y) {
	LayerMap[item_x][item_y].litem = add_item;
}

int layer::AddCharacter(character* add_char, int char_x, int char_y) {
	LayerMap[char_x][char_y].lchar = add_char;
}



// PRIVATE FUNCTIONS/HELPERS FOR GENERATION

void layer::Generate() {
	// GENERATE NUMBER OF ROOMS based on dimensions and ROOM_DENSITY variable
	int nrooms;
	nrooms = (WIDTH*HEIGHT*ROOM_DENSITY)/(MAX_ROOM_RADIUS*MAX_ROOM_RADIUS) + rand()%ROOM_DENSITY; 
		// This formula comes from trial and error. We can adjust it if need be.
	
	// CHOOSE LOCATIONS FOR ROOMS. 
	for(int x=0;x<WIDTH;x++) {
		for(int y=0;y<HEIGHT;y++) {
			LayerMap[x][y].room = -1; // set room ids to -1 to show that they belong to no room
		}
	}
	int roomloc[4][nrooms]; // Keep an array of room center locations and types
	
	for(int i=0;i<nrooms;i++) {
	
		roomloc[0][i] = rand()%(WIDTH-4) +2; // random x coordinate
		roomloc[1][i] = rand()%(HEIGHT-4) +2; // random y coordinate
		int x = roomloc[0][i];
		int y = roomloc[1][i];
		
		roomloc[2][i] = rand()%3;
		// possible values for roomloc[2][i]: 0,1,2 = rectangular, 3,4 = circular, 5=irregular
		
		// check if this location or an adjacent location already has a seed. If so, redo seeding.
		if(LayerMap[x][y].room == -1 && 
			LayerMap[x-1][y].room == -1 && 
			LayerMap[x+1][y].room == -1 && 
			LayerMap[x-1][y-1].room == -1 && 
			LayerMap[x][y-1].room == -1 && 
			LayerMap[x+1][y-1].room == -1 &&
		 	LayerMap[x-1][y+1].room == -1 && 
		 	LayerMap[x][y+1].room == -1 && 
		 	LayerMap[x+1][y+1].room == -1 ) {
				LayerMap[x][y].room = i;
			cout << "Seed: x:" << x << ", y:" << y << endl;
		}
		else
			i--;
	}
	cout << "Done seeding." << endl;
	
	////////////////   GROW ROOMS   /////////////////
	int x, y;
	int iter =0;
	while(iter < nrooms) { 													// run algorithm until all rooms have grown
		for(int i=0;i<nrooms;i++) { 										// iterate through list of rooms to grow
			x = roomloc[0][i];
			y = roomloc[1][i];
			if(rand()%ROOM_VAR == 0) { 									// sometimes don't expand a room for variation
				cout << "Growing room " << i << ":" << roomloc[2][i] << endl;
				if(roomloc[2][i] < 2) { 									// ROOM IS RECTANGULAR
					roomloc[2][i] = ExpandRec(x,y,i); 
					if(roomloc[2][i] == 6) {
						iter++;
						cout << "Done growing rec room " << i << endl;
					}
					// IF EXPANDREC DISCOVERS THAT THE ROOM CAN'T EXPAND MORE:
					// The expansion function changes the room type to 6
					// ( so next time this for loop is executed, this room will miss all if statements )
					// iter is incremented to show that one room has finished
				}
				else if(roomloc[2][i] ==2) { // ROOM IS CIRCULAR
					roomloc[2][i] = ExpandCircle(x,y,i);
					if(roomloc[2][i] == 8) {
						iter++;
						cout << "Done growing circular room "<< i << endl;
					}
				}
			}
		}
	}
	
	cout << "Done Growing Rooms." << endl;
	
	// IDENTIFY WALLS/FLOORS
	for(int x=0;x<WIDTH;x++) {
		for(int y=0;y<HEIGHT;y++) {
			if(LayerMap[x][y].room >= 0 ) // if location is a ROOM
				LayerMap[x][y].lterrain = FLOOR; // make it a ROOM! :D
			else if(LayerMap[x][y].room == -1)
				LayerMap[x][y].lterrain = WALL;
		}
	}
	
	int newnrooms;
	int door_density = DOOR_DENSITY;
	do {													// PLACE DOORS
		for(int n=0;n<nrooms;n++) {
			newnrooms = PlaceDoors(roomloc[0][n],roomloc[1][n],n,roomloc[2][n],nrooms);
		}
		door_density--;
	} while(/*!CheckConnections(newnrooms, roomloc[0][0], roomloc[1][0])*/door_density>0);			// CHECK CONNECTIVITY
	
	cout << "Placing ladders." << endl;
	PlaceLadders();
};

bool layer::CheckConnections(int newnrooms, int x0, int y0) {
	tree ConnectTree(0);
	node* current_node;
	current_node = ConnectTree.get_first();
	bool connected;
	// build a tree from the first room
	BuildConnections(ConnectTree, current_node, x0, y0);
	ConnectTree.print();
	// search tree for each room through nrooms
	for(int i=0;i<newnrooms;i++) {
		if(ConnectTree.search_tree(i) == false)
			return false;
	}
	return true;
};

void layer::BuildConnections(tree &ConnectTree, node* root, int x, int y) {
	int this_room = root->data;
	int lr=0;																// find dimensions
	while( (x-lr)>0 && LayerMap[x-lr-1][y].room == this_room)			
		lr++;	
	int rr=0;																
	while( (x+rr)<WIDTH-1 && LayerMap[x+rr+1][y].room == this_room)
		rr++;
	int tr=0;
	while( (y-tr)>0 && LayerMap[x][y-tr-1].room == this_room)
		tr++;
	int br=0;
	while( (y+br)<HEIGHT-1 && LayerMap[x][y+br+1].room == this_room)
		br++;
	
	if(LayerMap[x+rr-1][y+br-1].room == this_room) { 			// if the room is a rectangle
	
		for(int i=1;i<=rr;i++) {											// find all doors and BuildConnections for them
			if(LayerMap[x+i][y+br+1].lterrain == DOOR && (br != 0 || i!=0) ) {
				int next_room;
				int next_room_y;
				next_room = LayerMap[x+i][y+br+2].room;
				next_room_y = y+br+2;
				if(!ConnectTree.search_tree(next_room)) {
					ConnectTree.add_child(root, next_room);
					BuildConnections(ConnectTree, root->firstchild, x+i, next_room_y);
				}
			}
			if(LayerMap[x+i][y-tr-1].lterrain == DOOR && (tr != 0 || i!=0) ) {
				int next_room;
				int next_room_y;
				next_room = LayerMap[x+i][y-tr-2].room;
				next_room_y = y-tr-2;
				if(!ConnectTree.search_tree(next_room)) {
					ConnectTree.add_child(root, next_room);
					BuildConnections(ConnectTree, root->firstchild, x+i, next_room_y);
				}
			}
		}
		for(int i=1;i<=lr;i++) {
			if(LayerMap[x-i][y+br+1].lterrain == DOOR && (br != 0 || i != 0) ) {
				int next_room;
				int next_room_y;
				if(LayerMap[x-i][y+br+2].room != -2) {
					next_room_y = y+br+2;
					next_room = LayerMap[x-i][y+br+2].room;
				}
				else {
					next_room = LayerMap[x-i][y+br+3].room;
					next_room_y = y+br+3;
				}
				if(!ConnectTree.search_tree(next_room)) {
					ConnectTree.add_child(root, next_room);
					BuildConnections(ConnectTree, root->firstchild, x-i, next_room_y);
				}
			}
			if(LayerMap[x-i][y-tr-1].lterrain == DOOR && (tr != 0 || i != 0) ) {
				int next_room;
				int next_room_y;
				if(LayerMap[x-i][y-tr-2].room != -2) {
					next_room = LayerMap[x-i][y-tr-2].room;
					next_room_y = y-tr-2;
				}
				else {
					next_room = LayerMap[x-i][y-tr-3].room;
					next_room_y = y-tr-3;
				}
				if(!ConnectTree.search_tree(next_room)) {
					ConnectTree.add_child(root, next_room);
					BuildConnections(ConnectTree, root->firstchild, x-i, next_room_y);
				}
			}
		}
		for(int j=0;j<=br;j++) {
			if(LayerMap[x+rr+1][y+j].lterrain == DOOR && (rr != 0 || j != 0) ) {
				int next_room;
				int next_room_x;
				if(LayerMap[x+rr+2][y+j].room != -2) {
					next_room = LayerMap[x+rr+2][y+j].room;
					next_room_x = x+rr+2;
					}
				else {
					next_room = LayerMap[x+rr+3][y+j].room;
					next_room_x = x+rr+3;
				}
				if(!ConnectTree.search_tree(next_room)) {
					ConnectTree.add_child(root, next_room);
					BuildConnections(ConnectTree, root->firstchild, next_room_x, y+j);
				}
			}
			if(LayerMap[x-lr-1][y+j].lterrain == DOOR && (lr != 0 || j != 0) ) {
				int next_room;
				int next_room_x;
				if(LayerMap[x-lr-2][y+j].room != -2) {
					next_room = LayerMap[x-lr-2][y+j].room;
					next_room_x = x-lr-2;
					}
				else {
					next_room = LayerMap[x-lr-3][y+j].room;
					next_room_x = x-lr-3;
				}
				if(!ConnectTree.search_tree(next_room)) {
					ConnectTree.add_child(root, next_room);
					BuildConnections(ConnectTree, root->firstchild, next_room_x, y+j);
				}
			}
		}
		for(int j=1;j<=tr;j++) {
			if(LayerMap[x+rr+1][y-j].lterrain == DOOR && (rr != 0 || j != 0) ) {
				int next_room;
				int next_room_x;
				if(LayerMap[x+rr+2][y-j].room != -2) {
					next_room = LayerMap[x+rr+2][y-j].room;
					next_room_x = x+rr+2;
				}
				else {
					next_room = LayerMap[x+rr+3][y-j].room;
					next_room_x = x+rr+3;
				}
				if(!ConnectTree.search_tree(next_room)) {
					ConnectTree.add_child(root, next_room);
					BuildConnections(ConnectTree, root->firstchild, next_room_x, y-j);
				}
			}
			if(LayerMap[x-lr-1][y-j].lterrain == DOOR && (lr != 0 || j != 0) ) {
				int next_room;
				int next_room_x;
				if(LayerMap[x-lr-2][y-j].room != -2) {
					next_room = LayerMap[x-lr-2][y-j].room;
					next_room_x = x-lr-2;
				}
				else {
					next_room = LayerMap[x-lr-3][y-j].room;
					next_room_x = x-lr-3;
				}
				if(!ConnectTree.search_tree(next_room)) {
					ConnectTree.add_child(root, next_room);
					BuildConnections(ConnectTree, root->firstchild, next_room_x, y-j);
				}
			}
		}
	}
}

int layer::PlaceDoors(int x, int y, int i, int type, int nrooms) {
	int doors = 0;
	int newnrooms = nrooms;
	if(type==6) {															// FOR A RECTANGLE
		cout << "Placing doors on " << i << endl;
		int lr=0;																// find dimensions
		while( (x-lr)>0 && LayerMap[x-lr-1][y].room == i)			
			lr++;	
		int rr=0;																
		while( (x+rr)<WIDTH-1 && LayerMap[x+rr+1][y].room == i)
			rr++;
		int tr=0;
		while( (y-tr)>0 && LayerMap[x][y-tr-1].room == i)
			tr++;
		int br=0;
		while( (y+br)<HEIGHT-1 && LayerMap[x][y+br+1].room == i)
			br++;
			
		cout << "tr: " << tr << " br: " << br << " rr: " << rr << " lr: " << lr << endl;
		
		for(int j=0;j<=rr;j++) {								// place doors on top of rectangle
			if(rr>0 && rand()%((rr+doors)) == 0) {
				int hlength=0;
				//check for hall length
				while(y+br+hlength+1 < HEIGHT-1 && LayerMap[x+j-1][y+br+hlength+1].lterrain == WALL && 
				LayerMap[x+j+1][y+br+hlength+1].lterrain == WALL && 
				LayerMap[x+j][y+br+hlength+1].lterrain == WALL) {
					hlength++;
				}
				//determine end conditions/abort door if unfavorable
				if(LayerMap[x+j][y+br+hlength+1].lterrain == WALL) {
					hlength = 0;
				}
				if(hlength>0 && hlength != 2) {					// draw hall and doors
					LayerMap[x+j][y+br+1].lterrain = DOOR;
					for(int k=2;k<hlength;k++) {
						LayerMap[x+j][y+br+k].room = nrooms;
						LayerMap[x+j][y+br+k].lterrain = FLOOR;
					}
					LayerMap[x+j][y+br+hlength].lterrain = DOOR;
				}
				newnrooms++;
				doors++;
			}
		}
		for(int j=0;j<br;j++) {
			doors = 0;
			if(br>0 && rand()%((br+doors)) == 0) {
				int hlength=0;
				//check for hall length
				while(x+rr+hlength+1 < WIDTH-1 && LayerMap[x+rr+hlength+1][y+j-1].lterrain == WALL && 
				LayerMap[x+rr+hlength+1][y+j+1].lterrain == WALL && 
				LayerMap[x+rr+hlength+1][y+j].lterrain == WALL) {
					hlength++;
				}
				//determine end conditions/abort door if unfavorable
				if(LayerMap[x+rr+hlength+1][y+j].lterrain == WALL) {
					hlength = 0;
				}
				if(hlength>0 && hlength != 2) {					// draw hall and doors
					LayerMap[x+rr+1][y+j].lterrain = DOOR;
					for(int k=2;k<hlength;k++) {
						LayerMap[x+rr+k][y+j].room = nrooms;
						LayerMap[x+rr+k][y+j].lterrain = FLOOR;
					}
					LayerMap[x+rr+hlength][y+j].lterrain = DOOR;
				}
				newnrooms++;
				doors++;
			}
		}	
	}
	else if(type==8) {									// FOR A CIRCLE
		int radius = 0;
		int short_radius = 0;
		while( (x-radius)>0 && LayerMap[x-radius-1][y].lterrain == WALL)			
			radius++;
		while( (y+short_radius) < HEIGHT-1 && LayerMap[x+radius][y+short_radius+1].lterrain == WALL)	
			short_radius++;
			
		for(int j=0;j<=short_radius;j++) {
			doors = 0;
			if(short_radius>0 && rand()%(2*short_radius+doors) == 0) {
				int hlength=0;
				//check for hall length
				while(y+radius+hlength+1 < HEIGHT-1 && LayerMap[x+j-1][y+radius+hlength+1].lterrain == WALL && 
				LayerMap[x+j+1][y+radius+hlength+1].lterrain == WALL && 
				LayerMap[x+j][y+radius+hlength+1].lterrain == WALL) {
					hlength++;
				}
				//determine end conditions/abort door if unfavorable
				if(LayerMap[x+j][y+radius+hlength+1].lterrain == WALL) {
					hlength = 0;
				}
				if(hlength>0 && hlength != 2) {					// draw hall and doors
					LayerMap[x+j][y+radius+1].lterrain = DOOR;
					for(int k=2;k<hlength;k++) {
						LayerMap[x+j][y+radius+k].room = nrooms;
						LayerMap[x+j][y+radius+k].lterrain = FLOOR;
					}
					LayerMap[x+j][y+radius+hlength].lterrain = DOOR;
				}
				newnrooms++;
				doors++;
			}
			doors = 0;
			if(short_radius>0 && rand()%(2*short_radius+doors) == 0) {
				int hlength=0;
				//check for hall length
				while(x+radius+hlength+1 < WIDTH-1 && LayerMap[x+radius+hlength+1][y+j-1].lterrain == WALL && 
					LayerMap[x+radius+hlength+1][y+j+1].lterrain == WALL && 
					LayerMap[x+radius+hlength+1][y+j].lterrain == WALL) {
						hlength++;
				}
				//determine end conditions/abort door if unfavorable
				if(LayerMap[x+radius+hlength+1][y+j].lterrain == WALL) {
					hlength = 0;
				}
					if(hlength>0 && hlength != 2) {					// draw hall and doors
						LayerMap[x+radius+1][y+j].lterrain = DOOR;
						for(int k=2;k<hlength;k++) {
							LayerMap[x+radius+k][y+j].room = nrooms;
							LayerMap[x+radius+k][y+j].lterrain = FLOOR;
						}
						LayerMap[x+radius+hlength][y+j].lterrain = DOOR;
					}
					newnrooms++;
					doors++;
		
			}
		}		
	}
	else
		cout << "Could not place doors, not circle or rectangle" << endl;
	return nrooms;
};

void layer::Print() {
	/**/
	for(int x=0;x<WIDTH;x++) {
		for(int y=0;y<HEIGHT;y++) {
			if(LayerMap[x][y].room < 0)
				cout << "#";
			else
				cout << LayerMap[x][y].room;
		}
		cout << endl;
	}/**/
	/**/for(int x=0;x<WIDTH;x++) {
		for(int y=0;y<HEIGHT;y++) {
			if(LayerMap[x][y].lterrain == WALL)
				cout << "#";
			else if(LayerMap[x][y].lterrain == DOOR)
				cout << "+";
			else if(LayerMap[x][y].lterrain == LADDER)
				cout << ">";
			else
				cout << ".";
		}
		cout << endl;
	}/**/
};

int layer::ExpandCircle(int x0, int y0, int i) {
	bool expand = true;
	
	// find current radius
	int radius = 0;
	while(x0+radius<WIDTH-1 && LayerMap[x0+1+radius][y0].room == i) {
		radius++;
	}
	
	// check if the circle can grow
		radius+=2;
		if(x0+radius>=WIDTH || x0-radius<0 || y0+radius>=HEIGHT || y0-radius<0) {
			expand = false;
		}
		else {
			int f = 1 - radius;
			int ddF_x = 1;
			int ddF_y = -2 * radius;
			int x = 0;
			int y = radius;
		 
			for(int k=0;k<=radius;k++) {
				if((LayerMap[x0][y0+k].room != i && LayerMap[x0][y0+k].room != -1) ||
					(LayerMap[x0][y0-k].room != i && LayerMap[x0][y0-k].room != -1) ||
					(LayerMap[x0+k][y0].room != i && LayerMap[x0+k][y0].room != -1) ||
					(LayerMap[x0-k][y0].room != i && LayerMap[x0-k][y0].room != -1) ) {
						expand = false;
				}
			}
		 
			while(x < y) {
				if(f >= 0) {
					y--;
					ddF_y += 2;
					f += ddF_y;
				}
				x++;
				ddF_x += 2;
				f += ddF_x;
			
				for(int k=x;k>=0;k--) {
					for(int j=y;j>=0;j--) {
					 	if( (LayerMap[x0+k][y0+j].room != i && LayerMap[x0+k][y0+j].room != -1) ||
							 (LayerMap[x0-k][y0+j].room != i && LayerMap[x0-k][y0+j].room != -1) ||
							 (LayerMap[x0+k][y0-j].room != i && LayerMap[x0+k][y0-j].room != -1) ||
							 (LayerMap[x0-k][y0-j].room != i && LayerMap[x0-k][y0-j].room != -1) ||
							 (LayerMap[x0+j][y0+k].room != i && LayerMap[x0+j][y0+k].room != -1) ||
							 (LayerMap[x0-j][y0+k].room != i && LayerMap[x0-j][y0+k].room != -1) ||
							 (LayerMap[x0+j][y0-k].room != i && LayerMap[x0+j][y0-k].room != -1) ||
							 (LayerMap[x0-j][y0-k].room != i && LayerMap[x0-j][y0-k].room != -1) ) {
							 	expand = false;
						}
					}
				}
			}
		}
	
	// grow the circle
	if(expand) {
		radius=radius-1;
		int f = 1 - radius;
		int ddF_x = 1;
		int ddF_y = -2 * radius;
		int x = 0;
		int y = radius;
		 
		for(int k=0;k<=radius;k++) {
			LayerMap[x0][y0+k].room = i;
			LayerMap[x0][y0-k].room = i;
			LayerMap[x0+k][y0].room = i;
			LayerMap[x0-k][y0].room = i;
		}  	 
		while(x < y) {
			if(f >= 0) {
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			x++;
			ddF_x += 2;
			f += ddF_x;
		
			for(int k=x;k>=0;k--) {
				for(int j=y;j>=0;j--) {  
					LayerMap[x0+k][y0+j].room = i;
					LayerMap[x0-k][y0+j].room = i;
					LayerMap[x0+k][y0-j].room = i;
					LayerMap[x0-k][y0-j].room = i;
					LayerMap[x0+j][y0+k].room = i;
					LayerMap[x0-j][y0+k].room = i;
					LayerMap[x0+j][y0-k].room = i;
					LayerMap[x0-j][y0-k].room = i;
				}
			}
		}
	}
	
	// return 6 if the circle is done growing. If the complete circle is too small, make it a rectangle.
	if(expand)
		return 2;
	else {
		if(radius <= 3) {
			LayerMap[x0][y0+1].room = -1;
			LayerMap[x0][y0-1].room = -1;
			LayerMap[x0+1][y0].room = -1;
			LayerMap[x0-1][y0].room = -1;
			return 0;
		}
		return 8;
	}
};

int layer::ExpandRec(int x, int y, int i) {
	bool again_expand = false;
	bool expand; 
																			
	int lr=0;																// find left(lr), right(rr),
	int rr=0;																// top(tr) and bottom(br) radii
	int tr=0;
   int br=0;
	while( (x-lr)>0 && LayerMap[x-lr-1][y].room == i)			// increment each radius until the edge
		lr++;																	// of the existing shape is reached
	while( (x+rr)<WIDTH-1 && LayerMap[x+rr+1][y].room == i)
		rr++;
	while( (y-tr)>0 && LayerMap[x][y-tr-1].room == i)
		tr++;
	while( (y+br)<HEIGHT-1 && LayerMap[x][y+br+1].room == i)
		br++;
	
	//////////	CHECK/GROW THE RECTANGLE IN EACH DIRECTION  ////////////
	/// +X direction first (right)
	expand = true;
	rr++;
	if((x+rr+1)<WIDTH && (y-tr-1)>=0 && (y+br+1)<HEIGHT) {
		for(int j=0;j<=tr+1;j++) {
			if((LayerMap[x+rr][y-j].room != -1 && LayerMap[x+rr][y-j].room != i) ||
				(LayerMap[x+rr+1][y-j].room != -1 ) ) {
					expand = false;
			}
		}
		for(int j=0;j<=br+1;j++) {
			if((LayerMap[x+rr][y+j].room != -1 && LayerMap[x+rr][y+j].room != i) ||
				(LayerMap[x+rr+1][y+j].room != -1 ) ) {
					expand = false;
			}
		}
	}
	else {
		expand = false;
	}
	if(expand) {
		for(int j=0;j<=tr;j++) {
			LayerMap[x+rr][y-j].room = i;
		}
		for(int j=0;j<=br;j++) {
			LayerMap[x+rr][y+j].room = i;
		}
		again_expand = true;
	}
	rr--;
	
	/// -X direction (left)
	expand = true;
	lr++;
	if((x-lr-1)>=0 && (y-tr-1)>=0 && (y+br+1)<HEIGHT ) {
		for(int j=0;j<=tr+1;j++) {
			if((LayerMap[x-lr][y-j].room != -1 && LayerMap[x-lr][y-j].room != i) ||
				(LayerMap[x-lr-1][y-j].room != -1 ) ) {
					expand = false;
			}
		}
		for(int j=0;j<=br+1;j++) {
			if((LayerMap[x-lr][y+j].room != -1 && LayerMap[x-lr][y+j].room != i) ||
				(LayerMap[x-lr-1][y+j].room != -1 ) ) {
					expand = false;
			}
		}
	}
	else {
		expand = false;
	}
	if(expand) {
		for(int j=0;j<=tr;j++) {
			LayerMap[x-lr][y-j].room = i;
		}
		for(int j=0;j<=br;j++) {
			LayerMap[x-lr][y+j].room = i;
		}
		again_expand = true;
	}
	lr--;
	
	/// +Y direction (down)
	expand = true;
	br++;
	if((y+br+1)<HEIGHT && (x-lr-1)>=0 && (x+rr+1)<WIDTH) {
		for(int j=0;j<=lr+1;j++) {
			if((LayerMap[x-j][y+br].room != -1 && LayerMap[x-j][y+br].room != i) ||
				(LayerMap[x-j][y+br+1].room != -1 ) ) {
					expand = false;
			}
		}
		for(int j=0;j<=rr+1;j++) {
			if((LayerMap[x+j][y+br].room != -1 && LayerMap[x+j][y+br].room != i) ||
				(LayerMap[x+j][y+br+1].room != -1 ) ) {
					expand = false;
			}
		}
	}
	else {
		expand = false;
	}
	if(expand) {
		for(int j=0;j<=lr;j++) {
			LayerMap[x-j][y+br].room = i;
		}
		for(int j=0;j<=rr;j++) {
			LayerMap[x+j][y+br].room = i;
		}
		again_expand == true;
	}
	br--;
	
	/// -Y direction (up)
	expand = true;
	tr++;
	if((y-tr-1)>=0 && (x-lr-1)>=0 && (x+rr+1)<WIDTH) {
		for(int j=0;j<=lr+1;j++) {
			if((LayerMap[x-j][y-tr].room != -1 && LayerMap[x-j][y-tr].room != i) ||
				(LayerMap[x-j][y-tr-1].room != -1 ) ) {
					expand = false;
			}
		}
		for(int j=0;j<=rr+1;j++) {
			if((LayerMap[x+j][y-tr].room != -1 && LayerMap[x+j][y-tr].room != i) ||
				(LayerMap[x+j][y-tr-1].room != -1 ) ) {
					expand = false;
			}
		}
	}
	else
		expand = false;
	if(expand) {
		for(int j=0;j<=lr;j++) {
			LayerMap[x-j][y-tr].room = i;
		}
		for(int j=0;j<=rr;j++) {
			LayerMap[x+j][y-tr].room = i;
		}
		again_expand = true;
	}
	tr --;
	
	//////////// CLEAN UP CORNERS
	// clean up the leftover corners
	if(again_expand) {
		if(LayerMap[x+rr+1][y].room == i) {
			if(LayerMap[x][y-tr-1].room ==i && 
				LayerMap[x+rr+2][y-tr-2].room == -1 &&
				LayerMap[x+rr+2][y-tr-1].room == -1 &&
				LayerMap[x+rr+1][y-tr-2].room == -1 )
					LayerMap[x+rr+1][y-tr-1].room = i;
			if(LayerMap[x][y+br+1].room ==i && 
				LayerMap[x+rr+2][y+br+2].room == -1 &&
				LayerMap[x+rr+2][y+br+1].room == -1 &&
				LayerMap[x+rr+1][y+br+2].room == -1 )
					LayerMap[x+rr+1][y+br+1].room = i;
		}
		if(LayerMap[x-lr-1][y].room == i) {
			if(LayerMap[x][y-tr-1].room ==i && 
				LayerMap[x-lr-2][y-tr-2].room == -1 &&
				LayerMap[x-lr-2][y-tr-1].room == -1 &&
				LayerMap[x-lr-1][y-tr-2].room == -1 )
					LayerMap[x-lr-1][y-tr-1].room = i;
			if(LayerMap[x][y+br+1].room ==i && 
				LayerMap[x-lr-2][y+br+2].room == -1 &&
				LayerMap[x-lr-2][y+br+1].room == -1 &&
				LayerMap[x-lr-1][y+br+2].room == -1 )
					LayerMap[x-lr-1][y+br+1].room = i;
		}
	}
	
	
	if(again_expand)													// inform Generate that room not done growing
		return 0; 														// 0 = continue growing as a rectangle
	else {
		if((lr+rr)<2 || (tr+br)<2) {								// if room too small, remove
			cout << "room " << i << " too small, removed" << endl;
			for(int j=0;j<WIDTH;j++) {
				for(int k=0;k<HEIGHT;k++) {
					if(LayerMap[j][k].room == i)
						LayerMap[j][k].room = -1;
				}
			}
		}
		return 6; 														// 6 = room done growing
	}
};


void layer::PlaceLadders() {
	int ladder_up_x;
	int ladder_up_y;
	int ladder_down_x;
	int ladder_down_y;
	
	do {
		ladder_up_x = rand()%WIDTH;
		ladder_up_y = rand()%HEIGHT;
	} while( LayerMap[ladder_up_x][ladder_up_y].room < 0);
	
	int max_difference = (int)sqrt(pow(HEIGHT, 2)+pow(WIDTH, 2));
	int difference;
	
	do {
		do {
			ladder_down_x = rand()%WIDTH;
			ladder_down_y = rand()%HEIGHT;
		} while(ladder_down_y == ladder_up_y || ladder_down_x == ladder_up_x);
		difference = (int)sqrt( pow(ladder_up_x-ladder_down_x, 2) + pow(ladder_up_y-ladder_down_y, 2));
	} while( LayerMap[ladder_down_x][ladder_down_y].room < 0 || difference < max_difference/4);
	
	LayerMap[ladder_down_x][ladder_down_y].lterrain = LADDER;
	LayerMap[ladder_up_x][ladder_up_y].lterrain = LADDER;
}

