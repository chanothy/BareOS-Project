#include <interrupts.h>
#include <queue.h>
#include <semaphore.h>

semaphore_t sem_table[NSEM];

/*  Finds an unusued semaphore in the sem_table and returns it after  *
 *  resetting its values and marking it as used.                      */
int32 sem_create(int32 count) {
  char mask = disable_interrupts();
  for (int i = 0; i < NSEM; i++) {
    if (sem_table[i].state == S_FREE) {
      sem_table[i].state = S_USED;
      sem_table[i].count = count;
      restore_interrupts(mask);
      return i;
    }
  }

  restore_interrupts(mask);
  return -1;
}

/*  Marks a semaphore as free  */
int32 sem_free(uint32 sid) {
  if (sem_table[sid].state == S_FREE) {
    return -1;
  }
  return 0;
}

/*  Decrements the given semaphore if it is in use.  If the semaphore  *
 *  is less than 0, marks the thread as waiting and switches to        *
 *  another thread.                                                    */
int32 sem_wait(uint32 sid) {
  char mask;
  mask = disable_interrupts();


  restore_interrupts(mask);
  return 0;
}

/*  Increments the given semaphore if it is in use.  Resume the next  *
 *  waiting thread (if any).                                          */
int32 sem_post(uint32 sid) {
  char mask;
  mask = disable_interrupts();


  restore_interrupts(mask);
  return 0;
}
