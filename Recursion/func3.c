/***************************************/
/*file:func3.c	                       */ 
/*question 3                           */
/*functions file for main3 program     */
/***************************************/


#include "recstrings.h"


/******************rec_appear func******************/

int rec_appear(char *s, char c)
{
    
    if(*s =='\0')
        return 0;
    
    if(*s==c)
        return rec_appear(s+1,c)+1; /*if its the letter so count her and*/
                                /*call for the recu with the next letter*/
        return rec_appear(s+1,c);
}


/******************rec_comper func******************/

int rec_comper(char *s,char *t)
{
    if(*t=='\0')
        return 1;
    if(*s==*t)
        return rec_comper(s+1,t+1); /*if the letter is the same so inc s and inc t*/
        else
            return 0;
}


/******************rec_substring func******************/

int rec_substring(char *s,char *t)
{
    if(strlen(s) < strlen(t)) /*checking length*/
        return 0;
    if(rec_comper(s,t))
        return 1;
    else
        return rec_substring(s+1,t); /*inc s only to search t in s+1*/
}




/******************rec_reverse func******************/

void rec_reverse(char *s)
{
    if(*s=='\0')
        return;
    rec_reverse(s+1); /*inc the string*/
    putchar(*s); /*print the string when folding*/
}


/******************rec_sum func******************/

int rec_sum(char *s)
{
    int num;
    if(*s=='\0')
        return 0;
    if(isdigit(*s))
    {
        num=*s-'0';
        return rec_sum(s+1)+num; /*inc s and add the num if is digit*/
    }
    return rec_sum(s+1); /*if not so inc s only*/
}


/******************rec_polindrom func******************/

int rec_polindrom(char *s,char size)
{
    if(size<TWO) /*minimum polindrom*/
        return 1;
    
    if(!isalpha(*s)) 
        return rec_polindrom(s+1,size-1); /*if is not alpha for the first letter so inc s and dec size*/
        
        if(!isalpha(*(s+size-1))) /*if is not alpha for the last letter so dec size*/
            return rec_polindrom(s,size-1);
        
        if(tolower(*s)==tolower(*(s+size-1))) /*if bouth of the equal so inc s and dec size twice*/
            return rec_polindrom(s+1,size-TWO);
        
        return 0;
}