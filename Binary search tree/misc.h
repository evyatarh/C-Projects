#ifndef MISC_H
#define MISC_H

void*	AllocateMemory(unsigned int size, const char* file, int line_num);
void*	ReallocateMemory(void* mem, unsigned int size, const char* file, int line_num);
void    FreeMemory(void* mem);

#endif
