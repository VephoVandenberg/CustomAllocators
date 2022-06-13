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
    printf("Memory address is %x \n", a);
    *a = 35;
    printf("With value of %ld\n", *a);
    
    int *b = VPMalloc(sizeof(int));
    printf("Memory address is %x \n", b);
    *b = 44;
    printf("With value of %ld\n", *b);

    printf("Memory address of a is %x\n", a);

    short *c = VPMalloc(sizeof(short));
    printf("Memory address is %x \n", c);
    *c = 35;
    printf("With value of %d\n", *c);

    printf("Check if the memory remains the same\n");
    printf("Memory address of a is %x\n", a);
    printf("Memory address of b is %x\n", b);
    
}

