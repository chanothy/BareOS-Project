#include <barelib.h>
#include <syscall.h>
#include <thread.h>

/*  'resched' places the current running thread into the ready state  *
 *  and  places it onto  the tail of the  ready queue.  Then it gets  *
 *  the head  of the ready  queue  and sets this  new thread  as the  *
 *  'current_thread'.  Finally,  'resched' uses 'ctxsw' to swap from  *
 *  the old thread to the new thread.                                 */
int32 resched(void) {
  // stays the same throughout
  static uint32 threadCounter = 0;
  int oldThread = current_thread;

  for (int i = 0; i < NTHREADS; i++) { 
    int newThread = threadCounter % NTHREADS;
    if (thread_table[newThread].state == TH_READY) {
      if (thread_table[oldThread].state == TH_RUNNING) {
        thread_table[oldThread].state = TH_READY;
      }
      thread_table[newThread].state = TH_RUNNING;
      current_thread = newThread;
      ctxsw(&(thread_table[newThread].stackptr),&(thread_table[oldThread].stackptr));
      return 0;
    }
    threadCounter = threadCounter + 1;
  }

  return 0;
}
