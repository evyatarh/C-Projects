/********************************************/
/*file: mines.c                             */ 
/*this is a heder file for mines.c main	  	*/
/********************************************/

#ifndef _MINESWEEPER_H
#define _MINESWEEPER_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COMBINATIONS 8 /*define variable COMBINATIONS*/


typedef enum {FALSE=0,TRUE=1}BOOLEAN;



typedef struct /*struct spot*/
{
  int mines_area;
  BOOLEAN mine;
  BOOLEAN appears;
}spot;


spot **minesweeper_board;
int size; 
int appears_num; /*global varibles*/
static int x_coordinate[]={0,1,1,1,0,-1,-1,-1};/*coodinates x & y*/
static int y_coordinate[]={1,1,0,-1,-1,-1,0,1};


void game_moves(); /*function declaration*/
void devide_mines(); 
void allotment_board();
void fill_board();
void numerate_area(int x, int y);
void find_numbers_by_recursion(int x,int y);
void print_board();
BOOLEAN inside_board(int x, int y);
BOOLEAN scan_spots(int x, int y);

#endif