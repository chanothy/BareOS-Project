#include <barelib.h>
#include <thread.h>
#include <interrupts.h>
#include <syscall.h>


/*  Takes an index into the thread_table.  If the thread is TH_DEFUNCT,  *
 *  mark  the thread  as TH_FREE  and return its  `retval`.   Otherwise  *
 *  raise RESCHED and loop to check again later.                         */
byte join_thread(uint32 threadid) {
  char mask;
  mask = disable_interrupts();
  if (thread_table[threadid].state == TH_DEFUNCT) {
    thread_table[threadid].state = TH_FREE;
    // restore_interrupts(mask);

    return thread_table[threadid].retval;
  }
  else {
    while (thread_table[threadid].state != TH_DEFUNCT) {
      if (thread_table[threadid].state == TH_FREE) {
        return -1;
      }
      else {
        raise_syscall(RESCHED);

      }
    }
  }
  restore_interrupts(mask);
  return 0;
}
