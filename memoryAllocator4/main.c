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

typedef struct node
{
    struct node *prev;
    struct node *next;
    ui32 memSize;
    bool isFree;
}header_t;

typedef struct
{
    ui8 stack[STACK_SIZE];
    ui8 heap[HEAP_SIZE];
    ui8* heapBreak;
    
    header_t *start;
    header_t *end;

    struct 
    {
	ui8 *data;
	ui8 *bss;
	ui8 *text;
    }data_t;
}vm;

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

header_t *findFreeBlock(size_t size);
header_t *requestMemory(size_t size);

void test()
{
    int *var = VVMalloc(sizeof(ui32));
    *var = 64;
    
}

void *VVMalloc(ui32 size)
{
    if (size)
    {
	return NULL;
    }
    
    header_t *header = findFreeBlock(size);
    if (header)
    {
	
    }

    header = requestMemory(size);
    if (!header)
    {
	return NULL;
    }
}

void VVFree()
{
    
}

header_t *requestMemory(size_t size)
{
    if ((VM.heapBreak - VM.heap) + HEADER_SIZE + size >= HEAP_SIZE)
    {
	return NULL;
    }

    header_t *header;
    if (!VM.start)
    {
	VM.heapBreak = VM.heap;

	header = VM.heap;	   
	VM.start = header;
	VM.end = VM.start;

	VM.heapBreak += HEADER_SIZE + size;
	
	VM.start->next = NULL;
	VM.start->prev = NULL;
    }
    else
    {
	header = VM.heapBreak;
	if (VM.start == VM.end)
	{
	    VM.start->next = VM.end;
	    VM.start->prev = NULL;
	    
	    VM.end->prev = VM.start;
	    VM.end->next = NULL;
	}
	else
	{
	    endd
	}
    }
    
    header->memSize = size;
    header->isFree = FALSE;
}

header_t *findFreeBlock(size_t size)
{
    header_t *header = VM.start;

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
