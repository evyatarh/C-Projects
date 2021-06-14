#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "position.h"


  typedef enum {Up='i',Down='m',Left='j',Right='l'}Direction;

  /*this struct present a 'snake' with  length, position and direction/*/
  typedef struct Snake{

    int length;
    Direction calibrator;
    Position *pointer_object;

  }Snake;

  /************* functions delecration****************/
  Snake *CreateSnake(int snake_length, int x,int y);
  void DeleteSnake(Snake *p_snake);
  Snake* MoveSnake(Snake *p_snake,Direction direction_way);
  bool CheckCordinates(int x,int y);
  bool CheckLength(int length,int spot_x);

#endif
