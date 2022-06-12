#include <stdio.h>
#include "src/VPMemory.h"

void testAllocator();

int main(int argc, char **argv)
{

    return 0;
}

void testAllocator()
{
    
    int *a = VPMalloc(sizeof(int));
    printf("Memory address is %ld \n", a);
    *a = 35;
    printf("With value of %ld\n", *a);
    
    int *b = VPMalloc(sizeof(int));
    printf("Memory address is %ld \n", b);
    *b = 44;
    printf("With value of %ld\n", *b);

    short *c = VPMalloc(sizeof(short));
    printf("Memory address is %ld \n", c);
    *c = 35;
    printf("With value of %d\n", *c);
}

