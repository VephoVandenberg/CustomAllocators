#include <stdio.h>
#include "src/VPMemory.h"

void testAllocator();

int main(int argc, char **argv)
{
    testAllocator();
    return 0;
}

void testAllocator()
{
    
    int *a = VPMalloc(sizeof(int));
    printf("Memory address in 'a' is %p \n", a);
    *a = 35;
    printf("With value of %ld\n", *a);
    
    int *b = VPMalloc(sizeof(int));
    printf("Memory address in 'b' is %p\n", b);
    *b = 44;
    printf("With value of %ld\n", *b);


    short *c = VPMalloc(sizeof(short));
    printf("Memory address in 'c' is %p \n", c);
    *c = 35;
    printf("With value of %d\n\n", *c);

    printf("Now let's free the variable 'b'\n");
    VPFree(b);
    printf("The memory address in 'b' %p\n\n", b);
    
    printf("Check if the memory remains the same\n");
    printf("Memory address of a is %p\n", a);
    printf("Memory address of b is %p\n", b);
    printf("Memory address of c is %xpn\n", c);
    
    printList();

    printf("Free 'a' \n");
    VPFree(a);
    printList();
}

