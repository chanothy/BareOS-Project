#include <barelib.h>
#include <malloc.h>
#include <thread.h>
#include <bareio.h>
#include <barelib.h>

extern uint32 *mem_start;
extern uint32 *mem_end;
static alloc_t *freelist;

/*  Sets the 'freelist' to 'mem_start' and creates  *
 *  a free allocation at that location for the      *
 *  entire heap.                                    */
//--------- This function is complete --------------//
void heap_init(void)
{
  freelist = (alloc_t *)mem_start;
  freelist->size = get_stack(NTHREADS) - mem_start - sizeof(alloc_t);
  freelist->state = M_FREE;
  freelist->next = NULL;
}

/*  Locates a free block large enough to contain a new    *
 *  allocation of size 'size'.  Once located, remove the  *
 *  block from the freelist and ensure the freelist       *
 *  contains the remaining free space on the heap.        *
 *  Returns a pointer to the newly created allocation     */
void *malloc(uint64 size)
{
  alloc_t *prev = NULL;
  alloc_t *curr = freelist;

  while (curr != NULL)
  {
    if (curr->state == M_FREE && curr->size >= size)
    {
      // cut the block off and update new_free
      if (curr->size - size >= sizeof(alloc_t))
      {
        alloc_t *new_free = (alloc_t *)((char *)curr  + size + sizeof(alloc_t) ); // move to next mem
        new_free->size = curr->size - size - sizeof(alloc_t);
        new_free->state = M_FREE;
        new_free->next = curr->next;
        if (prev == NULL)
        {
          freelist = new_free;
        }
        else
        {
          prev->next = new_free;
        }
      }

      curr->size = size;
      curr->state = M_USED;
      curr->next = NULL;

      return (void *)((char *)curr + sizeof(alloc_t));
    }
    // update curr
    prev = curr;
    curr = curr->next;
    if (curr == NULL && prev != NULL)
    {
      prev->state = M_USED;
    }
  }
  return NULL;
}

/*  Free the allocation at location 'addr'.  If the newly *
 *  freed allocation is adjacent to another free          *
 *  allocation, coallesce the adjacent free blocks into   *
 *  one larger free block.                                */
void free(void *addr){

  alloc_t *toFree = (alloc_t *)((char *)addr - sizeof(alloc_t));

  if (toFree->state == M_FREE){
    return;
  }
  else{
    toFree->state = M_FREE;
    if ((char *)toFree < (char *)freelist){ 
      toFree->next = freelist;
      freelist = toFree;
    }
    else{ // free middle block and last block
      toFree->next = freelist->next;
      freelist->next = toFree;
    }
  }

  alloc_t *freeCo = (alloc_t *)((char *)addr - sizeof(alloc_t));

  if ((char *) freeCo + freeCo->size + sizeof(alloc_t) == (char *) freeCo->next) {
    freeCo->size += (freeCo->next)->size + sizeof(alloc_t);
    freeCo->next = (freeCo->next)->next;
  }

  alloc_t *curr = freelist;

  while (curr <= freeCo){ 
    if (curr->next == freeCo && (char *) curr + curr->size + sizeof(alloc_t) == (char *) freeCo) {
      curr->size += freeCo->size;
      curr->next = freeCo->next;
      return;
    }
    curr = curr->next;
  }

  return;
}
