#include <stdio.h>

#define ui8 unsigned char
#define ui16 unsigned short
#define ui32 unsigned int
#define ui64 unsigned long

#define bool ui8

#define i8 char
#define i16 short
#define i32 int
#define i64 long

#define STACK_SIZE 64
#define HEAP_SIZE STACK_SIZE * 8
#define TRUE 1
#define FALSE 0

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

typedef struct node
{
    struct node *prev;
    struct node *next;
    ui32 memSize;
    bool isFree;
}header_t;

#define HEADER_SIZE sizeof(header_t)

static vm VM;

header_t *start;
header_t *end;

void *VVMalloc(ui32 size);
void VVFree();

void test();

int main(int argc, char **argv)
{
    test();
    return 0;
}

header_t *findFreeBlock(size_t size);

void printObjInfo(void *ptr)
{
    
    printf("Heap base: %p\n", VM.heap);
    printf("Variable address: %p\n", ptr);
}

void test()
{
    int *var = VVMalloc(sizeof(ui32));
    *var = 64;
    printObjInfo(var);
}

void *VVMalloc(ui32 size)
{
    header_t *header;
    
    if (size <= 0)
    {
	return NULL;
    }
    else if (!VM.heapBreak)
    {
	VM.heapBreak = VM.heap;
	header = VM.heapBreak;
	header->prev = NULL;
	header->next = NULL;
	header->memSize = size;
	header->isFree = FALSE;
	return header + 1;
    }
    else if (VM.heapBreak + size >= HEAP_SIZE)
    {
	return NULL;
    }

    header = findFreeBlock(size);

    if (header)
    {
	header = VM.heapBreak;
	header->isFree = FALSE;
    }
    else
    {
	header = VM.heapBreak;
    }
}

void VVFree()
{
    
}

header_t *findFreeBlock(size_t size)
{
    header_t *header = (header_t *)VM.heap;

    while (header->next)
    {
	if (header->size >= size && header->isFree)
	{
	    return header;
	}
	header = header->next;
    }
    return NULL;
}
