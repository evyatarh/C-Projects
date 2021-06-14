/*****************************************************************/
/*file: main3.c                                                  */ 
/*question 3                                                     */
/*on this program we have five sections:                         */
/*1.Find with recursion how many times the specific letter showed*/
/*2.Find if one string is sub string of the seconed              */
/*3.Print the string backword.                                   */
/*4.Find the sum of the digits in the string.                    */
/*5.Check if the string is a polindrom.                          */ 
/*****************************************************************/

#include "recstrings.h"

int main()
{
    char string2[]="dm";
    char string1[]="step on No pets";
    char string3[]="evya7475tar";
    char c='o';
    int help=strlen(string1);
    
    printf("The letter '%c' showed : %d times\n",c,rec_appear(string1,c));
    printf("The the seconed string %s substring of the first.\n",rec_substring(string1,string2)?"is":"isn't");
    
    rec_reverse(string3);
    
    putchar('\n');
    printf("The sum of the digits in the string is: %d\n",rec_sum(string3));
    printf("The string '%s' is %s polindrom\n",string1,rec_polindrom(string1,help)?"a":"not a");
   
  return 0;
}
