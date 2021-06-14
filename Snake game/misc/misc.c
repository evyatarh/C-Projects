#include "misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <curses.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

  /*****************FreeMemory2D function*******************/
	
void* AllocateMemory(unsigned int size, const char* file, int line_num){

		void*	ptr = malloc(size);

		// If we failed the user will know exactly where the error happened
		if(ptr == NULL) {
			fprintf(stderr, "Fatal error in %s (%d): failed to allocate memory of size %u\n\n", file, line_num, size);
			exit(-1);
		}

		// Set all memory to 0
		memset(ptr, 0, size);

		return ptr;
}


  /*****************AllocateMemory2D function*******************/

void**	AllocateMemory2D(unsigned int nrows, unsigned int ncols, unsigned int element_size, const char* file, int line_num){
	/*This function allocate 2D memory for the screen dunctions*/
		void **Array2Demantion;
		void *Array2DemantionSec;
		int i;

		if((Array2Demantion=(void **)malloc(ncols*sizeof(void*)))==NULL){
			fprintf(stderr,"%s,line %d:failed to allocate %d bytes\n\n", file,line_num, element_size);
			exit(1);
		}

			if((Array2DemantionSec=(void *)malloc(ncols*nrows*element_size*sizeof(void)))==NULL){
				fprintf(stderr,"%s,line %d:failed to allocate %d bytes\n\n", file,line_num, element_size);
				exit(1);
			}
			for(i=0;i<ncols;i++)/*this loop matching between  each cell from  Array2Demantion to Array2DemantionSec*/
			Array2Demantion[i]=Array2DemantionSec+(nrows*i);

	return Array2Demantion;
}


  /*****************FreeMemory2D function*******************/

void	FreeMemory2D(void** mem){
		    free(*mem);
				free(mem);
}



  /*****************FreeMemory function*******************/
  
void    FreeMemory(void* mem){
	        free(mem);
}

  /*****************sleep_ms function*******************/
  
void sleep_ms(int milliseconds){ // cross-platform sleep function

	#ifdef WIN32
	        Sleep(milliseconds);
	#elif _POSIX_C_SOURCE >= 199309L
	        struct timespec ts;
	        ts.tv_sec = milliseconds / 1000;
	        ts.tv_nsec = (milliseconds % 1000) * 1000000;
					nanosleep(&ts, NULL);
	#else
	        usleep(milliseconds * 1000);
	#endif
}

  /*****************kbhit function*******************/
  
bool kbhit(void){

		struct termios oldt, newt;
		int ch;
		int oldf;

		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	  	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

		ch = getchar();

		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		fcntl(STDIN_FILENO, F_SETFL, oldf);

		if(ch != EOF)
		{
			ungetc(ch, stdin);
			return true;
		}

		return false;
}
