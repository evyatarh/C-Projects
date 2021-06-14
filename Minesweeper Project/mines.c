/**************************************************/
/*file: mines.c                                   */ 
/*the program is a minesweeper game.              */
/*the user enter size of board and coordinate and */
/*deside how many mines he want.                  */ 
/**************************************************/



#include "minesweeper.h"


int main()
{
  int mines_num;
  
  printf("please enter a number to define a board size:\n");
  scanf("%d",&size); /*collecting size from user*/
  printf("please enter a number of mines u want:\n");
  scanf("%d",&mines_num); /*collecting number of mines*/
  appears_num=(size*size)-mines_num;
  allotment_board();
  devide_mines(mines_num);
  game_moves();
  free(minesweeper_board);
  
  return 0;
}









