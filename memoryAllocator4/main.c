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
#define HEAP_SIZE STACK_SIZE * 8

typedef struct
{
    ui8 stack[STACK_SIZE];
    ui8 heap[HEAP_SIZE];
    ui8* heapBreak;
    
    struct 
    {
	ui8 *data;
	ui8 *bss;
	ui8 *text;
    }data_t;
}vm;

typedef struct
{
    ui32 prev;
    ui32 next;
}header_t;

#define HEADER_SIZE sizeof(header_t)

static vm VM;

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
    printf("%p\n", VM.heap);
    VVMalloc(23);
    printf("%p\n", VM.heapBreak);
}

void *VVMalloc(ui32 size)
{
    if (size <= 0)
    {
	return NULL;
    }
    
    void *header;
    void *memory;
    
    if (!VM.heapBreak)
    {
	VM.heapBreak = VM.heap;
    }

    header = VM.heapBreak;
    VM.heapBreak += HEADER_SIZE;
    memory = VM.heapBreak;
    VM.heapBreak += size;

    return memory;
}

void VVFree()
{
    
}
