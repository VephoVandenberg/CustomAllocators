#include <stdio.h>
#include <unistd.h>

#define ui8 unsigned char
#define ui16 unsigned short
#define ui32 unsigned int
#define ui64 unsigned long

#define i8 char
#define i16 short
#define i32 int
#define i64 long

typedef struct header
{
    ui8 isFree;
    ui32 prevLen;
    ui32 currLen;
} header_t;

#define HEADER_SIZE sizeof(header_t)
#defome FREE_LIST_SIZE 64

header_t *start;
header_t *end;

header_t freeList[FREE_LIST_SIZE];

void *VVMalloc(ui32 size);
void VVFree(void *pointer);

int main(int argc, char **argv)
{
    return 0;
}

void *VVMalloc(ui32 size)
{
    void *header = sbrk(0);
    void *memBlock = sbrk(size);

    if (memBlock == (void *)(-1))
    {
	return NULL;
    }
    
    return (header + 1);
}

void VVFree(void *pointer)
{
    
}

