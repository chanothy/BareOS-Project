#include <barelib.h>
#include <interrupts.h>
#include <bareio.h>

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
  restore_interrupts(mask);
  boot_complete = 1;
  
  printf("hello! %d\n",49);
  
}