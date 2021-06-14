/**************************************************/
/*file: mines_func.c                              */ 
/*this is a function file for mines.c main        */
/**************************************************/


#include "minesweeper.h"

/*********fill_board function*********/

void fill_board()
{
  int i,j; /*input variables*/
  for(i=0;i<size;i++)
  {
    for(j=0;j<size;j++)
    {
      minesweeper_board[i][j].mines_area=0;
      minesweeper_board[i][j].mine=FALSE;
      minesweeper_board[i][j].appears=FALSE;
    }
  }
}

/***********numerate_area function************/

void numerate_area(int x, int y)
{
  spot *point;
  int i,x_res,y_res;/*input variables*/
  
  for(i=0;i<COMBINATIONS;i++)
  {
    x_res=x+x_coordinate[i];
    y_res=y+y_coordinate[i];
    
    if(inside_board(x_res,y_res)==TRUE)
    {
      point=*(minesweeper_board+x_res)+y_res;
      
      if(point->mine==FALSE)
	(point->mines_area)++;
      
    }
  }
}


/***********scan_spots function************/

BOOLEAN scan_spots(int x, int y)
{
  if(minesweeper_board[x][y].appears==FALSE)
  {
    if(minesweeper_board[x][y].mine==TRUE)
    {
      printf("BOOMM!\nYOU LOST!!!\nmaybe try again later..\n");
      return TRUE;
    }
    else
    {
      if(minesweeper_board[x][y].mines_area==0)
	
	find_numbers_by_recursion(x,y);
      else
      {
      appears_num--;
	minesweeper_board[x][y].appears=TRUE;
     }
    }
  }
  return FALSE;
}

/***********inside_board function************/

BOOLEAN inside_board(int x, int y)
{
  if(x<size && x>=0 && y<size && y>=0)
    return TRUE;
  else
    return FALSE;
}



/***********devide_mines function************/

void devide_mines(int mines_num) /*distrebut mines in the board*/
{
  spot *temp;
  int i,x,y; /*input variables*/
  srand(time(0)); 
  
  for(i=0;i<mines_num;i++)
  {
    x=rand()%size; /*for random numbers*/
    y=rand()%size;
    temp=*(minesweeper_board+x)+y;
    if((temp->mine)==FALSE)
    {
      temp->mine=TRUE;
      numerate_area(x,y);
    }
    else
      i--;
  }
}


/***********allotment_board function************/

void allotment_board()
{
  int i;
  if((minesweeper_board=(spot**)malloc(size*sizeof(spot*)))==NULL)
  {
    printf("could not allocate memmory");
    exit(1);
  }
  
  for(i=0;i<size;i++)
  {
    if((minesweeper_board[i]=(spot*)malloc(size*sizeof(spot)))==NULL)
    {
      printf("could not allocate memmory");
      exit(1);
    }
  }
  fill_board();
}

/***********game_moves function************/

void game_moves()
{
  int x,y; /*input variables*/
  BOOLEAN explosion=FALSE;
  while(explosion==FALSE)
  {
    print_board();
    if(appears_num==0)
    {
    explosion=TRUE;
    printf("YOU WONNN!!\n");
    }
    else
    {
    printf("please chose your steps:(x and y)\n");
    scanf("%d %d",&x,&y); /*collecting coordinate*/
    
    
    if(inside_board(x-1,y-1)==1)
    {
      
      explosion=scan_spots(x-1,y-1);
    }
    else
      printf("illegal steps!!\nplease chose two numbers between 1-%d:\n",size);
   }
  }
  
}


/***********find_numbers_by_recursion function************/

void find_numbers_by_recursion(int x,int y)
{
  int i;
  if(inside_board(x,y)==TRUE && minesweeper_board[x][y].appears==FALSE)
  {
    minesweeper_board[x][y].appears=TRUE;
    appears_num--;
    if(minesweeper_board[x][y].mines_area==0)
      for(i=0;i<COMBINATIONS;i++)
	find_numbers_by_recursion(x_coordinate[i]+x,y_coordinate[i]+y);
  }
  
}


/***********print_board function************/

void print_board()
{
  int i,j,t,k; /*input variables*/
  char num;
  for(i=0;i<size;i++)
  {
    printf("\n   +");
    for(j=0;j<size;j++)
    {
      
      printf("---+");
    }
    putchar('\n');
    printf(" %d",i+1);
    for(t=0;t<size+1;t++)
    {
      num=minesweeper_board[i][t].appears;
      if(num==TRUE)
    	num=minesweeper_board[i][t].mines_area+'0';
        else
    	  num='X';
          if(t==size)
	        num=' ';
      printf(" | %c",num);
    }
    
  }
  
  putchar('\n');
  
  printf("   +");
  for(k=0;k<size;k++)
  {
    printf("---+");
  }
  
  putchar('\n');
  printf("  ");
    for(k=0;k<size;k++)
  {
    printf("   %d",k+1);
  }
  
  putchar('\n');
  
}