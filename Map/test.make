# test.make

test: main.o layer.o tree.o character.o item.o monster.o
	g++ main.o layer.o tree.o character.o item.o monster.o -lm -o test
	
main.o: main.cpp layer.h
	g++ -c main.cpp
	
layer.o: layer.cpp layer.h tree.h item.h character.h
	g++ -c layer.cpp 
	
tree.o: tree.cpp tree.h
	g++ -c tree.cpp
	
character.o: character.cpp character.h
	g++ -c character.cpp

item.o: item.cpp item.h
	g++ -c item.cpp

monster.o: monster.cpp monster.h
	g++ -c monster.cpp
