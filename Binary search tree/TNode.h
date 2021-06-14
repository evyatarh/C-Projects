#ifndef TNODE_H
#define TNODE_H

#include <stdbool.h>

typedef void* Element;
typedef struct TNode* TNode;



int Max (int n1, int n2);
TNode TNodeInsert(TNode tnode,Element key,bool (*is_smaller)(Element, Element),Element	(*copy)(Element));
/***************************************************************************************************************************************/
bool TNodeSearch(TNode tnode, Element key,bool (*is_smaller)(Element, Element));

/***************************************************************************************************************************************/
TNode TNodeRemove(TNode tnode, Element key, bool (*is_smaller)(Element, Element),void	(*free)(Element),Element (*copy)(Element) );
/***************************************************************************************************************************************/
void TNodePrintInorder(TNode tnode, void(*print)(Element));

/***************************************************************************************************************************************/
void TNodeDestroy(TNode tnode, void	(*free)(Element));
/***************************************************************************************************************************************/
Element TNodeMin(TNode tnode );
/***************************************************************************************************************************************/
Element TNodeMax(TNode tnode );	
/***************************************************************************************************************************************/
int TNodeSize(TNode tnode);
/***************************************************************************************************************************************/
int TNodeDepth (TNode tnode);

#endif
