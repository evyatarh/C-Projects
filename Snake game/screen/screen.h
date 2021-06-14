#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "position.h"
#include "snake.h"
#include "misc.h"

  /*this struct present a 'Screen' with row, col and 2D demantions array*/
  typedef struct Screen{

    int row;
    int col;
    char **pointer_chracters;

  }Screen;

  /************* functions delecration****************/
  Screen* CreateScreen(int rows,int cols);
  void PrintScreen(Screen *p_screen,FILE *fin);
  void DeleteScreen(Screen *p_screen);
  bool DrawScreen(Screen *p_screen,Snake *p_snake);


#endif
