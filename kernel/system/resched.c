#include <barelib.h>
#include <syscall.h>
#include <thread.h>
#include <queue.h>
#include <interrupts.h>

/*  'resched' places the current running thread into the ready state  *
 *  and  places it onto  the tail of the  ready queue.  Then it gets  *
 *  the head  of the ready  queue  and sets this  new thread  as the  *
 *  'current_thread'.  Finally,  'resched' uses 'ctxsw' to swap from  *
 *  the old thread to the new thread.                                 */
int32 resched(void) {
  // stays the same throughout
  // static uint32 threadCounter = 0;
  char mask;
  mask = disable_interrupts();

  int oldThread = current_thread;
  uint32 newThread = thread_dequeue(ready_list);


  if (newThread == NTHREADS) {
    resume_thread(current_thread);
    restore_interrupts(mask);
    return 0;
  }
  else {
    if (thread_table[newThread].state == TH_READY) {
      if (thread_table[oldThread].state == TH_RUNNING) {
        thread_table[oldThread].state = TH_READY;
      }
      thread_enqueue(ready_list, oldThread);
      current_thread = newThread;
      ctxsw(&(thread_table[newThread].stackptr),&(thread_table[oldThread].stackptr));
    }
  }

  restore_interrupts(mask);
  return 0;
}
