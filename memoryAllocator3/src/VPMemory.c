#include <stdio.h>
#include <string.h>
#include "VPMemory.h"

header_t *start = NULL;
header_t *end = NULL;

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

void fusion(header_t *header)
{
    if (header->next && header->next->isFree)
    {
	header->size += HEADER_SIZE + header->next->size;
	header->next = header->next->next;
    }
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
    
    if (header == end)
    {
	if (start == end)
	{
	    start = NULL;
	    end = NULL;
	}
	else
	{
	    header_t *prev = start;
	    while (prev)
	    {
		if (prev->next == end)
		{
		    prev->next = NULL;
		    end = prev;
		    return;
		}
		prev = prev->next;
	    }
	}
	sbrk(0 - header->size - HEADER_SIZE);
    }
    else
    {
	header_t *prev = start;
	while(prev)
	{
	    if (prev->next == end)
	    {
		break;
	    }
	    prev = prev->next;
	}

	header->isFree = TRUE;
	if (prev)
	{
	    fusion(prev);
	}
	if (header->next)
	{
	    fusion(header);
	}
    }
   
}

header_t *findFreeBlock(size_t size)
{
    header_t *current = start;
    while (current && (current->size >= size))
    {
	if (current->isFree)
	{
	    splitMemoryBlock(current, size);
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
	printf("Current address: %p\n", current);
	printf("Next address: %p\n", current->next);
	printf("Adress of actual memory: %p\n", (current + 1));
	printf("Size of allocated memory after the  header in bytes: %d\n", current->size);
	printf("Is this node free or not: %d\n\n", current->isFree);
	current = current->next;
    }
}
