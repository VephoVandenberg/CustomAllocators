#include "VPMemory.h"


header_t *start = NULL;
header_t *end = NULL;

header_t *getMemoryHeader(void *pointer)
{
    if (pointer)
    {
	return (header_t*)(pointer - 1);
    }
    return NULL;
}

void *VPMalloc(size_t size)
{
    header_t *block;

    if (size == 0)
    {
	return NULL;
    }

    block = findFreeBlock(size);

    if (!block)
    {
	block = requestSpace(size);
	if (!block) // Could not request space
	{
	    return NULL;
	}
    }
    else
    {
	block->isFree = FALSE;
    }

    return (block + 1);
}

{
    if (!pointer)
    {
	return VPMalloc(size);
    }

    header_t *header = getMemoryHeader(pointer);
    if (header->size >= size)
    {
	return pointer;
    }

    void *newMemory = VPMalloc(size);
    if (!newMemory)
    {
	return NULL;
    }
    memcpy(newMemory, pointer, header->size);
    VPFree(pointer);

    return newMemory;
}

void VPFree(void *pointer)
{
    if (!pointer)
    {
	return;
    }

    header_t *header = getMemoryHeader(pointer);
    if (header)
    {
	header->isFree = TRUE;
    }
}

header_t *findFreeBlock(size_t size)
{
    header_t *current = start;
    while (current && (current->size >= size))
    {
	if (current->isFree)
	{
	    return current;
	}
	current = current->next;
    }

    return NULL;
}

header_t *requestSpace(size_t size)
{
    header_t *block = sbrk(0);
    void *request = sbrk(HEADER_SIZE + size);

    // Failed to allocate memory
    if (request == (void*)-1)
    {
	return NULL;
    }
    
    block->isFree = FALSE;
    block->size = size;
    block->next = NULL;

    if (!start)
    {
	start = block;
    }
    end = block;

    return block;
}
