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

 
