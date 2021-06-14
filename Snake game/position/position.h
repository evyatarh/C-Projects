#ifndef POSITION_H
#define POSITION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

  /*This struct present a cordinates of a point in the space*/
  typedef struct Position{

    int cordinate_x;
    int cordinate_y;

  }Position;

  /************* functions delecration****************/
  Position *CreatePosition(int x,int y);
  void DeletePosition(Position *pointer_position);

#endif
