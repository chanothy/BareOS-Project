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

  // this checks if threadid is in the the list
  uint32 curr_thread = thread_queue[ready_list].qnext;
  while (curr_thread != ready_list && curr_thread != threadid)
  {
    curr_thread = thread_queue[curr_thread].qnext;
  }

  uint32 prev_thread = thread_queue[curr_thread].qprev;
  uint32 next_thread = thread_queue[curr_thread].qnext;
  thread_queue[curr_thread].qnext = NULL;
  thread_queue[curr_thread].qprev = NULL;
  thread_queue[prev_thread].qnext = next_thread;
  thread_queue[next_thread].qprev = prev_thread;

  // take thread out of list and add it to sleep
  if (curr_thread)
  {
    thread_queue[threadid].key = delay;
    thread_table[threadid].state = TH_SLEEP;

    thread_enqueue_sleep(sleep_list, threadid, delay);

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
    thread_queue[next_thread].key += thread_queue[curr_thread].key;
    thread_queue[curr_thread].qnext = NULL;
    thread_queue[curr_thread].qprev = NULL;
    thread_queue[prev_thread].qnext = next_thread;
    thread_queue[next_thread].qprev = prev_thread;
    thread_enqueue(ready_list, curr_thread);

  
    raise_syscall(RESCHED);
  }

  restore_interrupts(mask);
  return 0;
}
