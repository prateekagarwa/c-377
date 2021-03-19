#include "my_malloc.h"
#include <assert.h>
#include <stdio.h>
#include <sys/mman.h>

// A pointer to the head of the free list.
node_t *head = NULL;

// The heap function returns the head pointer to the free list. If the heap
// has not been allocated yet (head is NULL) it will use mmap to allocate
// a page of memory from the OS and initialize the first free node.
node_t *heap()
{
  if (head == NULL)
  {
    // This allocates the heap and initializes the head node.
    head = (node_t *)mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE,
                          MAP_ANON | MAP_PRIVATE, -1, 0);
    head->size = HEAP_SIZE - sizeof(node_t);
    head->next = NULL;
  }

  return head;
}

// Reallocates the heap.
void reset_heap()
{
  if (head != NULL)
  {
    munmap(head, HEAP_SIZE);
    head = NULL;
    heap();
  }
}

// Returns a pointer to the head of the free list.
node_t *free_list() { return head; }

// Calculates the amount of free memory available in the heap.
size_t available_memory()
{
  size_t n = 0;
  node_t *p = heap();
  while (p != NULL)
  {
    n += p->size;
    p = p->next;
  }
  return n;
}

// Returns the number of nodes on the free list.
int number_of_free_nodes()
{
  int count = 0;
  node_t *p = heap();
  while (p != NULL)
  {
    count++;
    p = p->next;
  }
  return count;
}

// Prints the free list. Useful for debugging purposes.
void print_free_list()
{
  node_t *p = heap();
  while (p != NULL)
  {
    printf("Free(%zd)", p->size);
    p = p->next;
    if (p != NULL)
    {
      printf("->");
    }
  }
  printf("\n");
}

// Finds a node on the free list that has enough available memory to
// allocate to a calling program. This function uses the "first-fit"
// algorithm to locate a free node.
//
// PARAMETERS:
// size - the number of bytes requested to allocate
//
// RETURNS:
// found - the node found on the free list with enough memory to allocate
// previous - the previous node to the found node
//
void find_free(size_t size, node_t **found, node_t **previous)
{

  node_t *prev = NULL;
  node_t *cur = heap();

  while (cur != NULL)
  {

    if ((cur->size + sizeof(node_t)) >= (size + sizeof(header_t)))
    {

      *found = cur;
      *previous = prev;
      break;
    }
    else
    {
      prev = cur;
      cur = cur->next;
    }
  }
}

// Splits a found free node to accommodate an allocation request.
//
// The job of this function is to take a given free_node found from
// `find_free` and split it according to the number of bytes to allocate.
// In doing so, it will adjust the size and next pointer of the `free_block`
// as well as the `previous` node to properly adjust the free list.
//
// PARAMETERS:
// size - the number of bytes requested to allocate
// previous - the previous node to the free block
// free_block - the node on the free list to allocate from
//
// RETURNS:
// allocated - an allocated block to be returned to the calling program
//
void split(size_t size, node_t **previous, node_t **free_block, header_t **allocated)
{
  assert(*free_block != NULL);

  node_t *og_next = (*free_block)->next;
  node_t *og_block = *free_block;
  size_t oldsize = (*free_block)->size;

  //adjust the free block pointer
  size_t actual_size = size + sizeof(header_t);
  *free_block = (node_t *)(((char *)*free_block) + actual_size);
  (*free_block)->next = og_next;
  (*free_block)->size = oldsize - size - sizeof(header_t);

  //step 2.
  if ((*previous) == NULL)
  {
    head = *free_block;
  }
  else
  {
    (*previous)->next = *free_block;
  }

  //step 3. allocate
  *allocated = (header_t *)og_block;
  (*allocated)->size = size;
  (*allocated)->magic = MAGIC;
}

// Returns a pointer to a region of memory having at least the request `size`
// bytes.
//
// PARAMETERS:
// size - the number of bytes requested to allocate
//
// RETURNS:
// A void pointer to the region of allocated memory
//
void *my_malloc(size_t size)
{

  node_t *found = NULL;
  node_t *previous = NULL;
  header_t *allocated = NULL;

  find_free(size, &found, &previous);
  if ((found) == NULL)
  {
    return NULL;
  }

  //if the size is exactly fit

  if (((found)->size + sizeof(node_t)) == (size + sizeof(header_t)))
  {
    //   // lay the header and information and set previous to null
    header_t *temp = (header_t *)found;
    temp->magic = MAGIC;
    temp->size = size;
    (previous)->next = NULL;
    return (((char *)temp) + sizeof(header_t));
  }
  //in case of head
  split(size, &previous, &found, &allocated);
  return (header_t *)(((char *)allocated) + sizeof(header_t));
}

// Merges adjacent nodes on the free list to reduce external fragmentation.
//
// This function will only coalesce nodes starting with `free_block`. It will
// not handle coalescing of previous nodes (we don't have previous pointers!).
//
// PARAMETERS:
// free_block - the starting node on the free list to coalesce
//
void coalesce(node_t *free_block)
{
  // when u add a block, add its size and then its next

  size_t newblockize = free_block->size;
  node_t *curblock = free_block;
  node_t *curnext = curblock->next;
  int count = 0;
  size_t block_size = curblock->size + sizeof(node_t);

  while ((((char *)curblock) + block_size == (char *)curnext) && curnext != NULL)
  {
    count += 1;
    newblockize += sizeof(node_t) + curnext->size;
    curblock = curnext;
    curnext = curblock->next;
    block_size = curblock->size + sizeof(node_t);
  }

  if (count > 0)
  {
    free_block->next = curnext;
    free_block->size = newblockize;
  }
}
// Frees a given region of memory back to the free list.
//
// PARAMETERS:
// allocated - a pointer to a region of memory previously allocated by my_malloc
//
void my_free(void *allocated)
{
  header_t *alloc = (header_t *)(((char *)allocated) - sizeof(header_t));

  assert(alloc->magic == MAGIC);
  size_t sizerep = alloc->size + sizeof(header_t) - sizeof(node_t);
  node_t *curnode = (node_t *)alloc;
  curnode->size = sizerep;
  curnode->next = head;
  head = curnode;
  coalesce(head);
}
