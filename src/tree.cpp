#ifndef TREE
#include "tree.h"
#define TREE
#endif


//-------------------------
// CONSTRUCTORS/DESTRUCTORS
//-------------------------
tree::tree () {
	first = new node;
}

tree::tree (int insert_data) {
	first = new node;
	first->data = insert_data;
}

void destructor_helper(node* root) {
	if(root->nxtsib != NULL)
		destructor_helper(root->nxtsib);
	if(root->firstchild != NULL)
		destructor_helper(root->firstchild);
	delete root;
}

tree::~tree () {
	destructor_helper(first);
}

//-----------
// FUNCTIONS
//-----------

void tree::add_child(node* root, int add_data) {
	node* temp;
	temp = root->firstchild;
	root->firstchild = new node(add_data);
	root->firstchild->nxtsib = temp;
}

node* tree::get_first() {
	return first;
}

bool search_helper(node* root, int search_for) {
	if(root->data == search_for)
		return true;
	if(root->nxtsib != NULL)
		if(search_helper(root->nxtsib, search_for))
			return true;
	if(root->firstchild != NULL)
		if(search_helper(root->firstchild, search_for))
			return true;
	return false;
}

bool tree::search_tree(int search_for) {
	return search_helper(first, search_for);
}

void print_helper(node* root, int depth) {
	cout << root->data << " : lvl " << depth << endl;
	if(root->nxtsib != NULL)
		print_helper(root->nxtsib, depth);
	if(root->firstchild != NULL)
		print_helper(root->firstchild, depth+1);
}

void tree::print() {
	int depth = 0;
	print_helper(first, depth);
}


node::node() {
	parent = NULL;
	prevsib = NULL;
	nxtsib = NULL;
	firstchild = NULL;
	data = 0;
};

node::node(int new_data) {
	parent = NULL;
	prevsib = NULL;
	nxtsib = NULL;
	firstchild = NULL;
	data = new_data;
};


