#include <interrupts.h>
#include <queue.h>
#include <thread.h>
#include <syscall.h>

/*  Places the thread into a sleep state and inserts it into the  *
 *  sleep delta list.                                             */
int32 sleep(uint32 threadid, uint32 delay) {
  char mask;
  mask = disable_interrupts();
  if (delay == 0) {
    raise_syscall(RESCHED);
    return 0;
  } 
  
  // loop thru ready_list. if threadid -> storePop else enqueue back
  uint32 originPop = thread_dequeue(ready_list);
  uint32 currPop = originPop;
  uint32 storePop;
  while (currPop != originPop) {
    if (currPop == threadid) {
      storePop = currPop;
    }
    else {
      thread_enqueue(ready_list, currPop);
    }
    currPop = thread_dequeue(ready_list);
  }
  if (storePop != threadid && currPop != NTHREADS) {
      thread_enqueue(ready_list, currPop);
  }

  thread_queue[storePop].key = delay;
  thread_enqueue(sleep_list, storePop);
  thread_table[storePop].state = TH_SLEEP;


  restore_interrupts(mask);
  return 0;
}

/*  If the thread is in the sleep state, remove the thread from the  *
 *  sleep queue and resumes it.                                      */
int32 unsleep(uint32 threadid) {
  char mask;
  mask = disable_interrupts();


  restore_interrupts(mask);
  return 0;
}
