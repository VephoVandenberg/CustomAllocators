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
    printf("Memory address of 'a' is 0X%x \n", a);
    *a = 35;
    printf("With value of %ld\n", *a);
    
    int *b = VPMalloc(sizeof(int));
    printf("Memory address of 'b' is 0X%x \n", b);
    *b = 44;
    printf("With value of %ld\n", *b);


    short *c = VPMalloc(sizeof(short));
    printf("Memory address of 'c' is 0X%x \n", c);
    *c = 35;
    printf("With value of %d\n\n", *c);

    printf("Now let's free the variable 'b'\n\n");
    VPFree(b);
    
    printf("Check if the memory remains the same\n");
    printf("Memory address of a is 0X%x\n", a);
    printf("Memory address of b is 0X%x\n", b);
    printf("Memory address of c is 0X%x\n\n", c);
    
    printList();
}
