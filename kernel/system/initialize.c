#include <barelib.h>
#include <interrupts.h>
#include <bareio.h>
#include <shell.h>
#include <thread.h>
#include <queue.h>
#include <malloc.h>
#include <tty.h>
#include <fs.h>

/*
 *  This file contains the C code entry point executed by the kernel.
 *  It is called by the bootstrap sequence once the hardware is configured.
 *      (see bootstrap.s)
 */

extern uint32* text_start;    /*                                             */
extern uint32* data_start;    /*  These variables automatically contain the  */
extern uint32* bss_start;     /*  lowest  address of important  segments in  */
extern uint32* mem_start;     /*  memory  created  when  the  kernel  boots  */
extern uint32* mem_end;       /*    (see mmap.c and kernel.ld for source)    */

void ctxload(uint64**);

uint32 boot_complete = 0;

void initialize(void) {
  char mask;
  mask = disable_interrupts();
  uart_init();
  heap_init();
  tty_init();

  bs_mk_ramdisk(MDEV_NUM_BLOCKS, MDEV_BLOCK_SIZE);
  fs_mkfs();
  fs_mount();

  for (int i = 0; i < NTHREADS; i++) {
    thread_table[i].state = TH_FREE;
  }

  thread_queue[ready_list].qnext = thread_queue[ready_list].qprev = ready_list;

  restore_interrupts(mask);
  boot_complete = 1;
  
  printf("Kernel start: %x\n",text_start);
  printf("--Kernel size: %d\n",(data_start - text_start));
  printf("Globals start: %x\n",data_start);
  printf("Heap/Stack start: %x\n",mem_start);
  printf("--Free Memory Available: %d\n", mem_end - mem_start);

  int32 shell_id = create_thread(shell,NULL,0);
  thread_table[shell_id].state = TH_RUNNING;
  ctxload(&(thread_table[current_thread].stackptr));
}
