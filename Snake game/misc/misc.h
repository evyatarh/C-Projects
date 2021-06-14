#ifndef MISC_H
#define	MISC_H

#include <stdbool.h>

  // Allocate memory for an array of size bytes, initialize to 0. Create an error message that includes
  // the file name (__FILE__) and line (__LINE__) from which the function was called
  void*	AllocateMemory(unsigned int size, const char* file_name, int line_num);

  // Free memory allocated by AllocateMemory()
  void	FreeMemory(void* mem);

  // Allocate a 2D memory as described in the assignment
  void**	AllocateMemory2D(unsigned int nrows, unsigned int ncols, unsigned int element_size, const char* file, int line_num);

  // Free memory allocated by AllocateMemory2D()
  void	FreeMemory2D(void** mem);

  // cross-platform sleep function
  void sleep_ms(int milliseconds);

  // Checks if a key was hit
  bool kbhit(void);

#endif
