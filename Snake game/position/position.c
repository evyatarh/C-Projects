#include "position.h"

  /*****************CreatePosition function*******************/

  /*This function get allocate a memory for one object of position.
  It gets two values of cordinates and update the cordinates in the object.*/

Position *CreatePosition(int x,int y){

    Position *p_position;

    p_position=(Position *)AllocateMemory(sizeof(Position),__FILE__, __LINE__);

    p_position->cordinate_x = x;
    p_position->cordinate_y = y;

    return p_position;
}

  /*****************DeletePosition function*******************/

void DeletePosition(Position *pointer_position){

    FreeMemory(pointer_position);
}
