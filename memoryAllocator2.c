#include <unistd.h>

typedef struct node
{
    size_t size;
    struct node *next;
    int is_free;
} node_t;

#define BLOCK_SIZE sizeof(node_t)

static node_t *start = NULL;
static node_t *end = NULL;


void *vv_malloc(size_t size);
void vv_free(void *memory_block); // TODO: add fusion
void split_memory(node_t *block, size_t size);
void *realloc(void *memory_block, size_t size); // TODO: implement later
node_t *fusion(node_t *block);  //TODO: implement later
node_t *give_more_memory(size_t size);
node_t *find_free_memory(size_t size);


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
            end = block;
        }
    }

    return (void *) (block + 1);    
}

void *realloc(void *memory_block, size_t size)
{
    if (!ptr)
        return malloc(size);
    

    node_t* node = (node_t *) memory_block - 1;

    if (node->size >= size)
    {
        split_memory(node, size);
        return memory_block;   
    }
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

    block->size = size;
    block->next = tail_block;
}

void vv_free(void *memory_block)
{
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
        node->is_free = 1;
    }
}
