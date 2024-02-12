#include <queue.h>

/*  Queues in bareOS are all contained in the 'thread_queue' array.  Each queue has a "root"
 *  that contains the index of the first and last elements in that respective queue.  These
 *  roots are  found at the end  of the 'thread_queue'  array.  Following the 'qnext' index 
 *  of each element, starting at the "root" should always eventually lead back to the "root".
 *  The same should be true in reverse using 'qprev'.                                          */

queue_t thread_queue[NTHREADS+1];   /*  Array of queue elements, one per thread plus one for the read_queue root  */
uint32 ready_list = NTHREADS + 0;   /*  Index of the read_list root  */


/*  'thread_enqueue' takes an index into the thread_queue  associated with a queue "root"  *
 *  and a threadid of a thread to add to the queue.  The thread will be added to the tail  *
 *  of the queue,  ensuring that the  previous tail of the queue is correctly threaded to  *
 *  maintain the queue.                                                                    */
void thread_enqueue(uint32 queue, uint32 threadid) {
  // if next and previous then part of q
  if (thread_queue[threadid].qnext != NULL || thread_queue[threadid].qprev != NULL)
    return;

  thread_t *thread = &thread_table[threadid];
  uint32 key = thread->priority;

  // keep iterating if key is lower
  uint32 current_thread = thread_queue[queue].qnext;
  while (current_thread != NULL && thread_table[current_thread].priority < key) {
    current_thread = thread_queue[current_thread].qnext;
  }

  if (current_thread != NULL) {
    uint32 prev = thread_queue[current_thread].qprev;
    thread_queue[prev].qnext = threadid;
    thread_queue[threadid].qnext = current_thread;
    thread_queue[threadid].qprev = prev;
    thread_queue[current_thread].qprev = threadid;
  }
  else {
    uint32 last = thread_queue[queue].qprev; // circular??
    thread_queue[last].qnext = threadid;
    thread_queue[threadid].qprev = last;
    thread_queue[threadid].qnext = NULL;
    thread_queue[queue].qprev = threadid;
  }
}


/*  'thread_dequeue' takes a queue index associated with a queue "root" and removes the  *
 *  thread at the head of the queue and returns the index of that thread, ensuring that  *
 *  the queue  maintains its structure and the head correctly points to the next thread  *
 *  (if any).                                                                            */
uint32 thread_dequeue(uint32 queue) {
  uint32 head = thread_queue[queue].qnext;
  // nothing in q
  if (head == NULL) {
    return 0;
  }
  uint32 next = thread_queue[head].qnext;
  if (next != NULL) {
    thread_queue[next].qprev = NULL;
  }
  else {
    thread_queue[queue].qprev = NULL;
  }
  thread_queue[head].qnext = NULL;
  thread_queue[head].qprev = NULL;
  return head;
}
