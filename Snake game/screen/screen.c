#include "screen.h"

  /***************** CreateScreen function*******************/

Screen* CreateScreen(int cols,int rows){

  Screen *p_screen;


    if(!CheckCordinates(rows,cols)){
      printf("The rows and the cols most be highr then 0\n");
      exit(1);
      }


      p_screen=(Screen*)AllocateMemory(rows*cols*sizeof(Screen),__FILE__, __LINE__);


      p_screen->pointer_chracters=(char**)AllocateMemory2D(rows,cols,sizeof(char),__FILE__, __LINE__);

      p_screen->row=rows;
      p_screen->col=cols;



      return p_screen;
}

  /***************** DrawScreen function*******************/

bool DrawScreen(Screen *p_screen,Snake *p_snake){

  int i,j;

  /*This loop fill the array with spaces*/
  for(i=0;i<p_screen->col;i++)
    for(j=0;j<p_screen->row;j++)
      p_screen->pointer_chracters[i][j]=' ';

  /*The head of the snake is located at the first index*/
  p_screen->pointer_chracters[p_snake->pointer_object[0].cordinate_x][p_snake->pointer_object[0].cordinate_y]='+';

  /*This loop draw the body of the snake*/

  for(i=p_snake->length-1;i>0;i--){
    if(p_screen->pointer_chracters[p_snake->pointer_object[i].cordinate_x][p_snake->pointer_object[i].cordinate_y]=='+')
      return false; /*if the head of the snake "crushed" into the rest of it body - return false-> will quit the program*/
    p_screen->pointer_chracters[p_snake->pointer_object[i].cordinate_x][p_snake->pointer_object[i].cordinate_y]='*';
    }


  /*This loop draw the first and the last line as part of the frame*/
  for(i=0;i<p_screen->col;i++){
     if(p_screen->pointer_chracters[i][0]=='+'||p_screen->pointer_chracters[i][p_screen->row-1]=='+')
             return false; /*if the head of the snake "crushed" into the frame - return false-> will quit the program*/
  p_screen->pointer_chracters[i][0]='#';
  p_screen->pointer_chracters[i][p_screen->row-1]='#';
    }

  /*This loop draw the first and the last colunms as part of the frame*/

  for(i=0;i<p_screen->row;i++){
     if(p_screen->pointer_chracters[0][i]=='+'||p_screen->pointer_chracters[p_screen->col-1][i]=='+')
             return false; /*if the head of the snake "crushed" into the frame - return false-> will quit the program*/
  p_screen->pointer_chracters[0][i]='#';
  p_screen->pointer_chracters[p_screen->col-1][i]='#';
    }


    return true;
}

  /***************** PrintScreen function*******************/

void PrintScreen(Screen *p_screen,FILE *fin){

    int i,j;

    for(i=0;i<p_screen->row;i++){
      for(j=0;j<p_screen->col;j++){
        fprintf(fin,"%c",p_screen->pointer_chracters[j][i]);
      }
      putchar('\n');
    }


}


  /***************** DeleteScreen function*******************/

void DeleteScreen(Screen *p_screen){

      FreeMemory2D((void**)p_screen->pointer_chracters);
      FreeMemory(p_screen);
}
