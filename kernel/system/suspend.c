#include <barelib.h>
#include <interrupts.h>
#include <syscall.h>
#include <thread.h>


/*  Takes a index into the thread table of a thread to suspend.  If the thread is  *
 *  not in the  running or  ready state,  returns an error.   Otherwise, sets the  *
 *  thread's  state  to  suspended  and  raises a  RESCHED  syscall to schedule a  *
 *  different thread.  Returns the threadid to confirm suspension.                 */
int32 suspend_thread(uint32 threadid) {
  char mask;
  mask = disable_interrupts();

  // thread_t currThr = thread_table[threadid]; // grabs thread from table
  // if (currThr != TH_RUNNING || currThr != TH_READY) {
  //   printf("error");
  //   return 1;
  // }
  // else {
  //   currThr = TH_SUSPEND;
  // }
  
  restore_interrupts(mask);
  return 0;
}
