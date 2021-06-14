#include "TNode.h"
#include "misc.h"
#include <stdio.h>
#include <stdlib.h>

/***************************************************************************************************************************************/
struct TNode {
	void* key;
	TNode left;	
	TNode right;
};

/***************************************************************************************************************************************/
TNode TNodeInsert(TNode tnode,Element key,bool (*is_smaller)(Element, Element),Element	(*copy)(Element)){
	
	if(!tnode){
		TNode new_node = (TNode)AllocateMemory(sizeof(struct TNode), __FILE__, __LINE__);
		new_node->key = copy(key);
   
		new_node->left = new_node->right = NULL;
		return new_node;
	}
	//if(key < (tnode->key)) // Go left
		if(is_smaller(key,(tnode->key)))
    tnode->left = TNodeInsert(tnode->left, key,is_smaller,copy);
	else if(!is_smaller(key,(tnode->key))) // Go right
		tnode->right = TNodeInsert(tnode->right, key,is_smaller,copy);
	// else: value is already in
	
	return tnode;
}

/***************************************************************************************************************************************/
bool TNodeSearch(TNode tnode, Element key,bool (*is_smaller)(Element, Element)){
	
	if(!tnode) // Tree is empty
		return false;
	//if(key == tnode->key) // Key was found
	if(!is_smaller(key,(tnode->key)) &&  !is_smaller((tnode->key),key))
		return true;
	//if(key < tnode->key) // Go left
	if(is_smaller(key,(tnode->key)))
		return TNodeSearch(tnode->left, key,is_smaller);
	// Go right
	return TNodeSearch(tnode->right, key,is_smaller);
}

/***************************************************************************************************************************************/
TNode TNodeRemove(TNode tnode, Element key, bool (*is_smaller)(Element, Element),void	(*fre)(Element),Element (*copy)(Element)){
	
	if(!tnode)
		return NULL;
	
		if(is_smaller(key,(tnode->key)))// Go left
				tnode->left=TNodeRemove(tnode->left, key,is_smaller,fre,copy);
			else if(is_smaller(tnode->key,key))// Go right
				tnode->right=TNodeRemove(tnode->right, key,is_smaller,fre,copy);
	else{
	
	// else – we need to remove this node – next slide
	// Option 1: trying to remove a leaf
	if(!(tnode->left) && !(tnode->right)) {
		fre(tnode->key);
		FreeMemory(tnode);
		
		return NULL;
	}
	// Option 2: node has only one child
	else if(!(tnode->left)) {
		TNode temp = tnode->right;
		fre(tnode->key);
		FreeMemory(tnode);
	
		return temp;
	}
	else if(!(tnode->right)) {
		TNode temp = tnode->left;
		fre(tnode->key);
		FreeMemory(tnode);
		
		return temp;
	}
	// Option 3: node has 2 children
	else {
		TNode x = tnode->right;
		TNode* xparent = &(tnode->right);
		while(x->left) {
			xparent=&(x->left); 
			x=x->left;
		}
		//tnode->key = x->key;
		tnode->key = copy(x->key);
		*xparent = TNodeRemove(x, x->key,is_smaller,fre,copy);
	 }
	}
	return tnode;
}

/***************************************************************************************************************************************/
void TNodePrintInorder(TNode tnode, void	(*print)(Element)) {
	
	if(tnode) {
		TNodePrintInorder(tnode->left,print);
		//printf("%d ", tnode->key);
			print(tnode->key);
		TNodePrintInorder(tnode->right,print);
	}
}

/***************************************************************************************************************************************/
void TNodeDestroy(TNode tnode, void	(*fre)(Element)) {
	if(tnode) {
		TNodeDestroy(tnode->left,fre);
		
		TNodeDestroy(tnode->right,fre);
		fre(tnode->key);
		
	}
}
/***************************************************************************************************************************************/
Element TNodeMin(TNode tnode ){
	if(tnode->left== NULL)
		return tnode->key;
	return TNodeMin(tnode->left);
}
/***************************************************************************************************************************************/
Element TNodeMax(TNode tnode ){
	if(tnode->right== NULL)
		return tnode->key;
	return TNodeMin(tnode->right);
}	
/***************************************************************************************************************************************/
int TNodeSize(TNode tnode){
	if(tnode==NULL)
		return 0;
	return (TNodeSize(tnode->left)+TNodeSize(tnode->right))+1;
}
/***************************************************************************************************************************************/
int TNodeDepth (TNode tnode){
	if(tnode==NULL)
		return 0;
	if(!(tnode->left)&&!(tnode->right))
		return 0;
	 return Max(TNodeDepth(tnode->left),TNodeDepth(tnode->right))+1;
}
/***************************************************************************************************************************************/
int Max (int n1, int n2){
return (n1>n2)? n1:n2;
}

