#include "snake.h"


  /*****************CreateSnake function*******************/

Snake *CreateSnake(int x,int y,int snake_length){

  Snake *p_snake;

  int i;

  if(!(CheckCordinates(x,y))){
    printf("Please do not enter a negative number!Try again\n");
    exit(1);
    }

  if(!(CheckLength(snake_length,x))){
    printf("Please enter a length even or smaller then cordinate x!Try again\n");
    exit(1);
    }

  p_snake=(Snake *)AllocateMemory(sizeof(Snake),__FILE__, __LINE__);

  p_snake->pointer_object=(Position *)AllocateMemory(snake_length*sizeof(Position),__FILE__, __LINE__);

  /*in this loop we create cordintes to each cell in the snake .*/
  for(i=0;i<=snake_length-1;i++)
    p_snake->pointer_object[i]=*(CreatePosition(x-i,y));


  p_snake->length=snake_length;
  p_snake->calibrator=Right;/*initialize the Direction to right.*/

  return p_snake;
  
}

  /*****************DeleteSnake function*******************/

void DeleteSnake(Snake *p_snake){

      DeletePosition(p_snake->pointer_object);
      FreeMemory(p_snake);
}

  /*****************MoveSnake function*******************/

Snake* MoveSnake(Snake *p_snake,Direction direction_way){

    int *xhelp;/*assistant arrays*/
    int *yhelp;
    int i;

    Direction face = p_snake->calibrator;


    /*allocate memory with AllocateMemory function*/
    xhelp=(int *)AllocateMemory(p_snake->length*sizeof(int),__FILE__, __LINE__);

    yhelp=(int *)AllocateMemory(p_snake->length*sizeof(int),__FILE__, __LINE__);

  /*If the Direction of the snake is opposite to the direction_way
  (the direction that the user press)ignore and continue in the same way.*/

    if((direction_way==Up && face==Down)||(direction_way==Down && face==Up))
        direction_way=face;

    if((direction_way==Left && face==Right)||(direction_way==Right && face==Left))
        direction_way=face;

    p_snake->calibrator=direction_way;/*else update the dirction.*/
  /*This loop copy the cordinates of the snake into the assistant arrays*/
  for(i=0;i<p_snake->length;i++){
    xhelp[i]=p_snake->pointer_object[i].cordinate_x;
    yhelp[i]=p_snake->pointer_object[i].cordinate_y;
    }

  /*change the head of the snake to the desired direction*/
    switch(direction_way){

    case Up:
    p_snake->pointer_object[0].cordinate_y--;
    break;

    case Down:
    p_snake->pointer_object[0].cordinate_y++;
    break;

    case Left:
    p_snake->pointer_object[0].cordinate_x--;
    break;

    case Right:
    p_snake->pointer_object[0].cordinate_x++;
    break;

  default:;
    }

  /*This loop upadate the cordinates from the assistant arrarys to the original array.*/
    for(i=1;i<p_snake->length;i++){
      p_snake->pointer_object[i].cordinate_x=xhelp[i-1];
      p_snake->pointer_object[i].cordinate_y=yhelp[i-1];
    }

    FreeMemory(xhelp);
    FreeMemory(yhelp);

      return p_snake;
}

  /*****************check_cordinates function*******************/

bool CheckCordinates(int x,int y){
    if(x<0 || y<0)
      return false;
      
      return true;
}

  /*****************check_length function*******************/

  /*This function make sure that the length of the snake is shorter then the width of the screen */
bool CheckLength(int length,int spot_x){
    return (length<=spot_x)?true:false;
}
