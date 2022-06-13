#include <stdio.h>
#include "VPMemory.h"

header_t *getMemoryHeader(void *pointer)
{
    if (pointer)
    {
	return (header_t*)pointer - 1;
    }
    return NULL;
}

void splitMemoryBlock(header_t *header, size_t size)
{
    if (!header || header->size <= size)
    {
	return;
    }
    
    header_t *newHeader = header + HEADER_SIZE + size;
    newHeader->size = header->size - HEADER_SIZE - size;
    newHeader->isFree = TRUE;
    header->size = size;
    newHeader->next = header->next;
    header->next = newHeader;
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

void *VPRealloc(size_t size, void *pointer)
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

void *VPCalloc(size_t number, size_t size)
{
    void *memory = VPMalloc(size * number);
    memset(memory, 0, size);
    return memory;
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
	pointer = NULL;
    }
}

header_t *findFreeBlock(size_t size)
{
    header_t *current = start;
    while (current && (current->size >= size))
    {
	if (current->isFree)
	{
	    splitMemoryBlock(current, size)
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
    if (end)
    {
	end->next = block;
    }
    end = block;

    return block;
}

void printList()
{
    header_t *current = start;

    while (current)
    {
	printf("Current address: 0X%x\n", current);
	printf("Next address: 0X%x\n", current->next);
	printf("Adress of actual memory: 0X%x\n", (current + 1));
	printf("Size of allocated memory after the  header in bytes: %d\n", current->size);
	printf("Is this node free or not: %d\n\n", current->isFree);
	current = current->next;
    }
}
