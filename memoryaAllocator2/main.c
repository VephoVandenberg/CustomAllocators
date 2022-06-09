#include <unistd.h>

typedef struct node
{
    size_t size;
    struct node *next;
    void *ptr;
    struct node *prev;
    int is_free;
} node_t;

#define BLOCK_SIZE sizeof(node_t)

static node_t *start = NULL;
static node_t *end = NULL;


void *vv_malloc(size_t size);
void vv_free(void *memory_block); 
void split_memory(node_t *block, size_t size);
void *vv_realloc(void *memory_block, size_t size); // TODO: implement later
void *vv_calloc(size_t number_of_elems, size_t size);
void copy_block(node_t *source, node_t *destenation);
node_t *fusion(node_t *block);
node_t *give_more_memory(size_t size);
node_t *find_free_memory(size_t size);
int valid_addr(void *ptr);


int main(int argc, char const *argv[])
{
    int *a = vv_malloc(sizeof(int));
    int *b = vv_malloc(sizeof(int));
    char *c = vv_malloc(sizeof(char));


    *a = 25;
    *b = 45;
    *c = 55;

    vv_free(b);
    vv_free(c);
    
    int *v = vv_realloc(a, 4);
    return 0;
}

node_t *give_more_memory(size_t size)
{
    node_t *node = (node_t *)sbrk(0);
    node_t *temp_var = (node_t *)sbrk(BLOCK_SIZE + size);

    if (!node || !temp_var) // in case sbrk() fails
        return NULL;
    
    // every time we make size of the used heap bigger,
    // we keep track of the last block, in this case
    // on the block we've just allocated space 
    node->is_free = 0;
    node->next = NULL;
    node->size = size;

    return node;
}

node_t *find_free_memory(size_t size)
{
    node_t *temp = start;

    while (temp)
    {
        if (temp->is_free)
        {
            if (temp->size > size)
            {
                split_memory(temp, size);
                temp->is_free = 0;
                break;
            }
            else if (temp->size == size)
            {
                temp->is_free = 0;
                break;
            }
        }
        temp = temp->next;
    }
    return temp;
    
}

void *vv_malloc(size_t size)
{
    node_t *block;

    if (!start) // In case nothing was allocated on heap
    {
        block = give_more_memory(size); 
        if (!block)
            return NULL;
        start = end = block;
    }
    else
    {
        block = find_free_memory(size);
        if (!block)
        {
            block = give_more_memory(size);
            if (!block) 
                return NULL;
            end->next = block;
            block->prev = end;
            end = block;
        }
    }
    block->ptr = (block + 1);
    return block->ptr;    
}

node_t *fusion(node_t *block)
{
    if (block->next && block->next->is_free)
    {
        block->size += BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
    }
    return block;
}

void *vv_calloc(size_t number_of_elems, size_t size)
{
    size_t *new, new_size;
    size_t i;

    new_size = number_of_elems * size;
    new = vv_malloc(new_size);
    if (new)
        for (i = 0; i < new_size; i++)
            new[i] = 0;
    return new;
}

void *vv_realloc(void *memory_block, size_t size)
{
    size_t s;
    node_t *old, *new;

    void *new_memory_ptr;

    if (!memory_block)
        return vv_malloc(size);

    old = (node_t *) memory_block - 1;    
    if (!valid_addr(memory_block))
        return NULL;

    if (old->size >= size && old->size - size >= BLOCK_SIZE)
        split_memory(old, size);
    else
    {
        if (old->next && old->next->is_free && (old->size + old->next->size + BLOCK_SIZE >= size))
        {
            fusion(old);
            if (old->size >= size && old->size - size >= BLOCK_SIZE)
                split_memory(old, size);
        }
        else
        {
            new_memory_ptr = vv_malloc(size);
            if (!new)
                return NULL;
            new = (node_t *) new_memory_ptr - 1;
            copy_block(old, new);
            vv_free(old);
            return new_memory_ptr;
        }
        
    }
    return memory_block;
    
}

void split_memory(node_t *block, size_t size)
{
    if (size >= block->size)
        return;

    node_t *tail_block;

    tail_block = block + BLOCK_SIZE + size;
    tail_block->size = block->size - size - BLOCK_SIZE;
    tail_block->is_free = 1;
    tail_block->next = block->next;
    tail_block->prev = block;

    block->size = size;
    block->next = tail_block;
}

void vv_free(void *memory_block)
{
    if (!valid_addr(memory_block)) // In case the address of the memory block is not valid
        return;

    node_t *temp;
    node_t *node = (node_t *) memory_block - 1;
    void *mem_chunk_break = sbrk(0);

    if (!memory_block)
        return;
    
    if (memory_block + node->size == mem_chunk_break)
    {
       if (start == end)
           start = end = NULL;
       else
       {
           temp = start;
           while (temp)
           {
               if (temp->next == end)
               {
                   temp->next = NULL;
                   end = temp;
               }
               temp = temp->next;
           }
       }
       sbrk(0 - BLOCK_SIZE - node->size);
    }
    else
    {
        // fusion with previous block if possible
        if (node->prev && node->prev->is_free)
            node = fusion(node->prev); 
        // then try with the next one
        if (node->next)
            node = fusion(node);
        node->is_free = 1;
    }
}

int valid_addr(void *p)
{
    node_t *block = p - BLOCK_SIZE;

    if (start)
        if ( p > start && p < sbrk(0))
            return (p == block->ptr);
    return 0;
}

void copy_block(node_t *source, node_t *destenation)
{
    size_t i;
    char *src, *dest;

    src = source->ptr;
    dest = destenation->ptr;

    for (i = 0; i < source->size && i < destenation->size; i++)
       src[i] = dest[i];
}
