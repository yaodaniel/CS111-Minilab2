#include "schedos-app.h"
#include "x86sync.h"

/*****************************************************************************
 * schedos-1
 *
 *   This tiny application prints red "1"s to the console.
 *   It yields the CPU to the kernel after each "1" using the sys_yield()
 *   system call.  This lets the kernel (schedos-kern.c) pick another
 *   application to run, if it wants.
 *
 *   The other schedos-* processes simply #include this file after defining
 *   PRINTCHAR appropriately.
 *
 *****************************************************************************/

#ifndef PRINTCHAR
#define PRINTCHAR	('1' | 0x0C00)
#endif
#ifndef MYPRIORITY
#define MYPRIORITY	1 //Exercise 4A, used to demonstrate my sys_set_priority system call
			  //schedos-x.c has their own definitions
#endif
#ifndef MYTIMESHARE
#define MYTIMESHARE	1 //Exercise 4B, used to demonstrate my sys_set_time_share system call
			  //schedos-x.c has their own definitions
#endif
//#define EXERCISE8	  //Uncomment for extra-credit exercise8 testing

void
start(void)
{
	int i;
	//sys_set_priority(MYPRIORITY); //Set this process' priority. Exercise 4A (scheduling_algorithm 2)
	//sys_set_time_share(MYTIMESHARE); //Set this process' time Share. Exercise 4B (scheduling_algorith 3)
	for (i = 0; i < RUNCOUNT; i++) {
		// Write characters atomically to the console, yielding after each one.
		#ifdef EXERCISE8
		sys_print_char((uint16_t)PRINTCHAR); //Exercise 8, uncomment the MACRO defined above first
		#else
		while(atomic_swap(&lock,1)!=0) { //Polling for the global lock. Exercise 6
			continue;
		}
		*cursorpos++ = PRINTCHAR;
		atomic_swap(&lock,0); //Release the lock Exercise 6
		#endif
		sys_yield();
	}

	// Yield forever.
	//while (1)
		//sys_yield();
		sys_exit(0);
}
