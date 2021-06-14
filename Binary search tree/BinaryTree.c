#include "BinaryTree.h"
#include "misc.h"
#include <stdio.h>
#include <stdlib.h>

/***************************************************************************************************************************************/
struct Tree {
	TNode	root;					// Actual root for the tree
	bool	(*is_smaller)(Element, Element);	// Returns true if the first element is smaller than the second one or false otherwise
	Element	(*copy)(Element);			// Creates a copy of the argument and returns it 
	void	(*free)(Element);			// Free an element created using copy
	void	(*print)(Element);			// Print an element
};

/***************************************************************************************************************************************/
Tree BSTCreate(bool (*is_smaller)(Element, Element), Element (*copy)(Element), void (*fre)(Element), void (*print)(Element))
{
	Tree tree = (Tree)AllocateMemory(sizeof(struct Tree), __FILE__, __LINE__);
	tree->root = NULL;
	tree->is_smaller = is_smaller;
	tree->copy = copy;
	tree->free = fre;
	tree->print = print;

	return tree;
}

/**********************************************************************, *****************************************************************/
void BSTDestroy(Tree tree) {
	TNodeDestroy( tree->root, tree->free);
	free(tree);
}

/***************************************************************************************************************************************/
void BSTInsert(Tree tree, Element e) {
	tree->root=TNodeInsert(tree->root,e, tree->is_smaller, tree->copy);
}

/***************************************************************************************************************************************/
void BSTRemoveNode(Tree tree, Element e) {
	tree->root= TNodeRemove(tree->root, e,tree->is_smaller,tree->free,tree->copy);
}

/***************************************************************************************************************************************/
bool BSTSearchNode(Tree tree, Element e)
{
	return TNodeSearch(tree->root, e,tree->is_smaller);
}

/***************************************************************************************************************************************/
int BSTSize(Tree tree)
{
	return TNodeSize(tree->root);
}

/***************************************************************************************************************************************/
int BSTDepth(Tree tree)
{
	return TNodeDepth (tree->root);
}

/***************************************************************************************************************************************/
void BSTPrintInorder(Tree tree) {
	 TNodePrintInorder(tree->root, tree->print);
}

/***************************************************************************************************************************************/
Element BSTMin(Tree tree)
{
	return TNodeMin(tree->root);
}

/***************************************************************************************************************************************/
Element BSTMax(Tree tree)
{
	return TNodeMax(tree->root);
}
