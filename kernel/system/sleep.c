#include <interrupts.h>
#include <queue.h>
#include <thread.h>
#include <syscall.h>

/*  Places the thread into a sleep state and inserts it into the  *
 *  sleep delta list.                                             */
int32 sleep(uint32 threadid, uint32 delay)
{
  char mask;
  mask = disable_interrupts();
  if (delay == 0)
  {
    raise_syscall(RESCHED);
    restore_interrupts(mask);
    return 0;
  }

  thread_table[threadid].state = TH_SLEEP;

  uint32 curr_thread = thread_queue[ready_list].qnext;
  while (curr_thread != ready_list && curr_thread != threadid)
  {
    curr_thread = thread_queue[curr_thread].qnext;
  }

  // take thread out of list and add it to sleep
  if (curr_thread)
  {
    uint32 prev_thread = thread_queue[curr_thread].qprev;
    uint32 next_thread = thread_queue[curr_thread].qnext;
    thread_queue[curr_thread].qnext = NULL;
    thread_queue[curr_thread].qprev = NULL;
    thread_queue[prev_thread].qnext = next_thread;
    thread_queue[next_thread].qprev = prev_thread;
    thread_enqueue(sleep_list, curr_thread);
    thread_queue[curr_thread].key = delay - thread_queue[thread_queue[curr_thread].qprev].key;
  }

  // adjust all keys after
  uint32 threadPtr = thread_queue[curr_thread].qnext;
  while (threadPtr != sleep_list) {
    thread_queue[threadPtr].key = thread_queue[threadPtr].key - thread_queue[thread_queue[threadPtr].qprev].key;
    threadPtr = thread_queue[threadPtr].qnext;
  }

  restore_interrupts(mask);
  return 0;
}

/*  If the thread is in the sleep state, remove the thread from the  *
 *  sleep queue and resumes it.                                      */
int32 unsleep(uint32 threadid)
{
  char mask;
  mask = disable_interrupts();

  if (thread_table[threadid].state != TH_SLEEP) {
    restore_interrupts(mask);
    return -1;
  }
  
  uint32 curr_thread = thread_queue[sleep_list].qnext;
  while (curr_thread != ready_list && curr_thread != threadid)
  {
    curr_thread = thread_queue[curr_thread].qnext;
  }

  // take thread out of list and add it to sleep
  if (curr_thread)
  {
    uint32 prev_thread = thread_queue[curr_thread].qprev;
    uint32 next_thread = thread_queue[curr_thread].qnext;
    thread_queue[curr_thread].qnext = NULL;
    thread_queue[curr_thread].qprev = NULL;
    thread_queue[prev_thread].qnext = next_thread;
    thread_queue[next_thread].qprev = prev_thread;
    thread_enqueue(ready_list, curr_thread);
    raise_syscall(RESCHED);
  }

   // adjust all keys after
  uint32 threadPtr = thread_queue[curr_thread].qnext;
  while (threadPtr != sleep_list) {
    thread_queue[threadPtr].key = thread_queue[threadPtr].key - thread_queue[thread_queue[threadPtr].qprev].key;
    threadPtr = thread_queue[threadPtr].qnext;
  }



  restore_interrupts(mask);
  return 0;
}
