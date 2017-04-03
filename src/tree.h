#ifndef TREE_H
#define TREE_H

#include <cstdlib>
#include <iostream>

using namespace std;

// A node of the tree which is built to contain room IDs.
class node {
	public:
		node();
		node(int);
		node* parent;
		node* prevsib;
		node* nxtsib;
		node* firstchild;
		node* lastchild;
		int data;
};


// A tree built to contain room IDs to check the connectivity of rooms
// on the map. A player must be able to get to every room, so a tree built
// from any room whose child nodes represent rooms attached to a parent node
// should contain every room ID (except rooms that were removed during construction)
class tree {
	public:
		// CONSTRUCTORS
		tree();
		tree(int);
		~tree();
		
		// FUNCTIONS
		// return the root of the tree
		node* get_first();
		// add a child containing the data given in the second argument
		// to the parent node given in the first argument
		void add_child(node*, int);
		// search the tree for the data given in single int argument. 
		// return true if found, false otherwise.
		bool search_tree(int search_for);
		// print the tree to console (used for debugging)
		void print();
		
		// FRIEND FUNCTIONS
		friend void destructor_helper(node* root);
		friend void print_helper(node* root, int depth);
		friend bool search_helper(node* root, int search_for);
	private:
		node* first;
};

#endif
