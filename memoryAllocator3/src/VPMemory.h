#include <unistd.h>

typedef struct header
{
    struct header *next;
    size_t size;
    char isFree;
}header_t;

#define HEADER_SIZE sizeof(header_t)

void *VPMalloc(size_t size);
void VPFree(void *pointer);

header_t *findFreeBlock(size_t size);
header_t *requestSpace(size_t size);
