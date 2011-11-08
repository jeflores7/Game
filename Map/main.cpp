#include <cstdlib>
#include <iostream>
#include <time.h>
#include "layer.h"

using namespace std;

int main () {
	srand(time(NULL));

	layer* map;
	map = new layer();
	map->InitMap();
	
	map->Print();

	delete map;

	return 0;
}
