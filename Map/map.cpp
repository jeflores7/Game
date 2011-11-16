#include "map.h"

using namespace std;

map::map() {
	clayer = new layer;
	clayer->Generate();
}

map::~map() {
	delete clayer;
}


int map::ReloadLevel() {
	clayer = clayer->
}

int map::MoveObject() {

}

int map::DeleteObject() {

}

int map::AddObject() {

}

int map::IdentifyObject() {

}

void map::test() {
	clayer->Print();
}

int map::GUIstringArrayLength(){
	return this->clayer->getHeight();
}
 
string* map::getGUIstrings(){
	string* GUIstring = new string[this->GUIstringArrayLength()];
	for (int i=0;i<this->GUIstringArrayLength();i++){
		for(int j=0;j<this->clayer->getWidth();j++){
			if(this->clayer->getLayerMap()[i].lchar!=0)
				GUIstring[i].append("@");
			else if (this->clayer->getLayerMap()[i].litem != 0)
				GUIstring[i].append("i");
			else if (this->clayer->getLayerMap()[i].lterrain == WALL)
				GUIstring[i].append("#");
			else
				GUIstring[i].append(" ");
		}
	}
	return GUIstring;
}
