/***************************************/
/*file:func4.c                         */ 
/*question 4                           */
/*function file for main4 program      */
/***************************************/

#include "pask.h"


/******************paskal func******************/

int paskal(int n,int k)
{
    if(n==1&&k==1)
        return 1;
    
    else if(k==0)
        return 0;
    
    else if(k>n)
        return 0;
    
    return (paskal(n-1,k-1)+paskal(n-1,k)); /*by the binomial coefficients */
}