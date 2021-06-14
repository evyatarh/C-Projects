#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "TNode.h"

/***************************************************************************************************************************************
 * This module represents a binary search tree. It contains the tree itself (which consists of objects of type TNode) but the interna 
 * structure is not exposed to the outside world 
 ***************************************************************************************************************************************/
typedef struct Tree* Tree;

// Constructor
Tree	BSTCreate(bool (*is_smaller)(Element, Element), Element (*copy)(Element), void (*fre)(Element), void (*print)(Element));
// Destructor
void	BSTDestroy(Tree tree);
// Add an element
void	BSTInsert(Tree tree, Element e);
// Remove an element
void	BSTRemoveNode(Tree tree, Element e);
// Find whether an element exists in the tree
bool	BSTSearchNode(Tree tree, Element e);
// Return the number if nodes in the tree
int	BSTSize(Tree tree);
// Return tree depth
int	BSTDepth(Tree tree);
// Print all node values in an inorder order
void	BSTPrintInorder(Tree tree);
// Return the minimum element
Element	BSTMin(Tree tree);
// Return the maximum element
Element	BSTMax(Tree tree);

#endif
