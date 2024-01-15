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

  // uint32* test = (uint32)0x23
  printf("Kernel start: %x\n",text_start);
  printf("--Kernel size: %d\n",(text_start-data_start));
  printf("Globals start: %x\n",data_start);
  printf("Heap/Stack start: %x\n",mem_start);
  printf("--Free memory available: %d\n",(mem_start-mem_end));

  
  // printf("\n");

  // printf("hex: %x feet\n\n", 0x2ab30);

  printf("hex: %x feet\n\n", 34000);
}
