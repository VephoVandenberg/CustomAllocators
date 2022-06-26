#include <stdio.h>

#define ui8 unsigned char
#define ui16 unsigned short
#define ui32 unsigned int
#define ui64 unsigned long

#define i8 char
#define i16 short
#define i32 int
#define i64 long

#define STACK_SIZE 64
#define HEAP_SIZE 128

typedef struct
{
    ui8 stack[STACK_SIZE];
    ui8 heap[HEAP_SIZE];

    struct 
    {
	ui8 *data;
	ui8 *bss;
	ui8 *text;
    }data_t;
}vm;

typedef struct
{
    unsigned int prev;
    unsigned int next;
}header_t;

static vm MainMemory;

void *VVMalloc(ui32 size);
void VVFree();

void test();

int main(int argc, char **argv)
{
    test();
    return 0;
}

void test()
{
    
}

void *VVMalloc(ui32 size)
{
    
}

void VVFree()
{
    
}
