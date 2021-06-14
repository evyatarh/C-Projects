#include "misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************************************************************/
void* AllocateMemory(unsigned int size, const char* file, int line_num)
{
        void*   ptr = malloc(size);

        // If we failed the user will know exactly where the error happened
        if(ptr == NULL) {
                fprintf(stderr, "Fatal error in %s (%d): failed to allocate memory of size %u\n\n", file, line_num, size);
                exit(-1);
        }

        // Set all memory to 0
        memset(ptr, 0, size);

        return ptr;
}

/*******************************************************************************************************************************/
void* ReallocateMemory(void* mem, unsigned int size, const char* file, int line_num)
{
        void*   ptr = realloc(mem, size);

        // If we failed the user will know exactly where the error happened
        if(ptr == NULL) {
                fprintf(stderr, "Fatal error in %s (%d): failed to allocate memory of size %u\n\n", file, line_num, size);
                exit(-1);
        }

        return ptr;
}

/*******************************************************************************************************************************/
void    FreeMemory(void* mem)
{
        free(mem);
}
