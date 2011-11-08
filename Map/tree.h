#ifndef TREE_H
#define TREE_H

#include <cstdlib>
#include <iostream>

using namespace std;

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

class tree {
	public:
		tree();
		tree(int);
		~tree();
		friend void destructor_helper(node* root);
		node* get_first();
		void add_child(node*, int);
		bool search_tree(int search_for);
		void print();
		friend void print_helper(node* root, int depth);
		friend bool search_helper(node* root, int search_for);
	private:
		node* first;
};

#endif
