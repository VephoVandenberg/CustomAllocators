#include <stdio.h>
#include <unistd.h>

/* We need an interface to access next memory segments
   to keep track if the memory segment is free
   and to have a size of memory block next to the header */
typedef struct header 
{
	unsigned size;
	unsigned is_free;
	struct  header *next;
} header_t;

static header_t *start; // start if our list in heap 
static header_t *end;	// end of list in heap

header_t *get_free_block(unsigned size); // Looking through list for the block of certain size
void *vp_malloc(unsigned size); // Our actual memory allocatr
void vp_free(void *block);	 // Free given block in heap

int main(int argc, char *argv[])
{
	int *a = vp_malloc(sizeof(int));
	int *b = vp_malloc(sizeof(int));

	*a = 16;
	*b = 256;

	printf("Adress %ld memory %d\n", a, *a);
	printf("Adress %ld memory %d\n", b, *b);
}

header_t *get_free_block(unsigned size)
{
	header_t *curr = start;

	while (curr)
	{
		if (curr->size >= size && curr->is_free)
			return curr;
		curr = curr->next;
	}

	return NULL;
}

void* vp_malloc(unsigned size)
{
	header_t *header;
	void *block;
	unsigned total_size;	

	if (!size)	// if the given size is zero
		return NULL;
	

	header = get_free_block(size); // first of all looking for a free block
	if (header)					   // if we find such we just set is_free to zero
	{
		header->is_free = 0;
		return (void*)(header + 1);	// and return a block of memory next to header
	}

	total_size = size + sizeof(header_t); // also if we could not find a free block we should make one
	block = sbrk(total_size);

	if (block == (void*)-1) // if something went wrong
		return NULL;
	

	header = block;
	header->is_free = 0;
	header->size = size;
	header->next = NULL;

	if (!start) // if the list we free we set the first element to the start
		start = header;
	

	if (end) // if the end already exists
		end->next = header; // we just tie to the end our new allocated block of memory

	return (void*)(header + 1);
}

void vp_free(void *block)
{
	header_t *header;
	header_t *temp;
	void *program_break;

	if (!block) // In case block is NULL
		return;
	

	program_break = sbrk(0);
	header = (header_t*)(block - 1);

	if ((char*)block + header->size == program_break)
	{
		if (start == end)
			start = end = NULL;
		else
		{
			temp = start;
			while(temp)
			{
				if (temp->next == end)
				{
					temp->next = NULL;
					end = temp;
				}

				temp = temp->next;
			}
		}
		sbrk(0 - sizeof(header_t) - header->size);
	}
	else
		header->is_free = 1;
}
