/*********************************************************/
/*file: main4.c                                          */ 
/*question 4                                             */
/*the program get two numbers (row and index),           */
/*and print the value that is in the index place.        */
/*the calculate work by the binomial coefficients        */
/*and its call paskal triangle                           */
/*********************************************************/

#include "pask.h"


int main()
{
    int k,n,ret=0;	
    
    printf("please enter two numbers (first=>second>0):\n");
    scanf("%d %d",&n,&k);
    ret=paskal(n,k); /*put in ret the return value from the func*/
    
    
    while(!ret) /*if ret is zero so the numbers i sent to the func not good so enter again*/
    {
        printf("ERROR!! first number must be greater then the seconed and\nthe seconed must be greater then zero.\n");
        printf("please enter the numbers again:\n");
        scanf("%d %d",&n,&k);
        ret=paskal(n,k);
    }
    
    printf("The number in the %d place is %d.\n",k,ret);   
    return 0;
}


