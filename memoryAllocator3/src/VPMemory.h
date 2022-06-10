#include <unistd.h>

typedef struct header
{
    struct header *next;
    size_t size;
    char isFree;
}header_t;

#define HEADER_SIZE sizeof(header_t)
#define TRUE 1
#define FALSE 0

void *VPMalloc(size_t size);
void VPFree(void *pointer);

header_t *findFreeBlock(size_t size);
header_t *requestSpace(size_t size);
