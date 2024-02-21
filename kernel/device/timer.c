/*
 *  This file contains functions for initializing and handling interrupts
 *  from the hardware timer.
 */

#include <barelib.h>
#include <interrupts.h>
#include <thread.h>
#include <queue.h>

#define TRAP_TIMER_ENABLE 0x80

volatile uint32 *clint_timer_addr = (uint32 *)0x2004000;
const uint32 timer_interval = 100000;
int64 resched(void);

/*
 * This function is called as part of the bootstrapping sequence
 * to enable the timer. (see bootstrap.s)
 */
void clk_init(void)
{
  *clint_timer_addr = timer_interval;
  set_interrupt(TRAP_TIMER_ENABLE);
}

/*
 * This function is triggered every 'timer_interval' microseconds
 * automatically.  (see '__traps' in bootstrap.s)
 */
interrupt handle_clk(void)
{
  *clint_timer_addr += timer_interval;

  // decremen head
  if (thread_queue[sleep_list].qnext != sleep_list) {
    thread_queue[thread_queue[sleep_list].qnext].key--;
  }

  // dequeue head if 0
  uint32 head = thread_queue[sleep_list].qnext;
  while (head != sleep_list) {
    uint32 next = thread_queue[head].qnext;
    if (thread_queue[head].key == 0) {
        uint32 threadid = thread_dequeue(sleep_list);
        thread_enqueue(ready_list, threadid);
        thread_table[threadid].state = TH_READY;
    }
    head = next;
  }


  if (boot_complete && is_interrupting()) {
    char mask = disable_interrupts();

    resched();
    restore_interrupts(mask);
  }
}
