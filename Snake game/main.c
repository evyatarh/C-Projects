#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "misc.h"
#include "position.h"
#include "screen.h"
#include "snake.h"

 /*****************main function*******************/
 
int main(){

		// Create a screen
		Screen* screen = CreateScreen(120, 40);
		// Create a snake of size 30
		Snake* snake = CreateSnake(60, 20, 30);
		// Initial direction of movement is Right
		Direction direction = Right;


		// Make sure the STDIN buffer is empty
		fflush(stdin);

		// As long as we can draw a legal screen, keep on going
		while(DrawScreen(screen, snake)) {
			system("clear");
			// Print the current screen
			PrintScreen(screen, stdout);
			// Wait 0.2 seconds
			sleep_ms(200);
			// If the user hit a key - read it and change direction if the key represent a legal direction
			if(kbhit()) {
				char ch = getchar();
				if((ch == Left) || (ch == Right) || (ch == Up) || (ch == Down)) {
					direction = (Direction)ch;
				}
				// Else - we will leave the original direction
			}
			MoveSnake(snake, direction);
		}

		DeleteScreen(screen);
		DeleteSnake(snake);

		return 0;
}
