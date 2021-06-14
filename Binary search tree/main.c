#include "BinaryTree.h"
#include "Date.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/***************************************************************************************************************************************/
bool IsSmaller(Element a, Element b){
	
	Date one=(Date)a;
	Date two=(Date)b;
	
	if(DateGetYear(one)<DateGetYear(two))
		return true;
	else if(DateGetYear(one)>DateGetYear(two))
		return false;
		
	if(DateGetMonth(one)<DateGetMonth(two))
		return true;
	else if(DateGetMonth(one)>DateGetMonth(two))
		return false;
	
	if(DateGetDay(one)<DateGetDay(two))
		return true;
	
	return false;

}
Element CopyDate(Element e){

	Date d = (Date)e;
	return(Element)DateCopy(d);
}


void FreeDate(Element e){
	
	Date d = (Date)e;
	DateDestroy(d);
}

void PrintDate(Element e){
	
	Date d = (Date)e;

	printf("%d/%d/%d\n",DateGetDay(d),DateGetMonth(d),DateGetYear(d));
	
}
/***************************************************************************************************************************************/
int main()
{
	char* buf = NULL;
	size_t buf_size = 0;
	bool (*is_smaller)(Element, Element)=IsSmaller;
	Element (*copy)(Element)=CopyDate;
	void (*free)(Element)=FreeDate;
	void (*print)(Element)=PrintDate;
	
	Tree tree = BSTCreate(is_smaller,copy,free,print);
	
	while(getline(&buf, &buf_size, stdin) != EOF) {
		char* p = strchr(buf, '\n');
		
		if(p)
			*p = '\0';
		if(strlen(buf) == 0)
			continue;
		Date d = DateCreate(buf);
		BSTInsert(tree, d);
		DateDestroy(d);
	}
	
	
	if(!BSTSize(tree)){
		printf("The Tree Is Empty!\n");
		exit(1);
	}
	printf("\nTree size:     %d\n", BSTSize(tree));
	printf("Tree depth:    %d\n", BSTDepth(tree));
	printf("Earliest date: ");
	PrintDate(BSTMin(tree));
	printf("\nLatest date:   ");
	PrintDate(BSTMax(tree));
	printf("\n\nAll dates (sorted):\n\n");
	BSTPrintInorder(tree);
	
	printf("\nDate To Search: 22/6/1941\n\n");
	Date d =DateCreate("22/6/1941");	
	printf("\nIs The Date Exist? %s \n",BSTSearchNode(tree,d)==true?"true":"false");
	BSTRemoveNode(tree, d);
	DateDestroy(d);
	printf("\nThe Size After Remove Is %d, And The Dates Are(Sorted):\n\n",BSTSize(tree));
	BSTPrintInorder(tree);
	BSTDestroy(tree);

	return 0;
}
