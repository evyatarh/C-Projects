/************************************/
/*file:recstrings.h                 */ 
/*question 3                        */
/*header file for main3 program     */
/************************************/


#ifndef _RECSTRINGS_H
#define _RECSTRINGS_H


#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define TWO 2

/***********functions delecrations************/

int rec_appear(char *s, char c);
int rec_substring(char *s,char *t);
int rec_comper(char *s,char *t);
void rec_reverse(char *s);
int rec_sum(char *s);
int rec_polindrom(char *s,char size);

#endif