
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "libs/types.h"
#include "fs/files.h"
#include "drivers/rtc.h"
#include "drivers/keyboard.h"
#include "user.h"
#include "paging.h"
#include "x86_desc.h"

// null-terminating character for strings
#define NULL_CHAR       '\0'

// magic numbers signifying it is an ELF executable
#define NUM_MAGIC_NUMS     4
#define MAGIC_NUM_1     0x7f
#define MAGIC_NUM_2     0x45
#define MAGIC_NUM_3     0x4c
#define MAGIC_NUM_4     0x46

#define RAMSIZE             (void *)0x100000

#define MAX_NUMBER_ARGS	  32

#define OPEN  			   0
#define READ  			   1
#define WRITE  			   2
#define CLOSE  			   3

#define IN_USE			   1
#define NOT_USE			   0

#define RTC_TYPE           0
#define DIR_TYPE           1
#define FILE_TYPE          2

#define MAX_FILES		   8

#define PROGRAM_LOCATION_MASK	0x80000000
#define MAX_PROG_NUM	32

#define MAX_RUN_PROG	3

#define NUM_BYTES_STATS   28
#define ENTRY_POINT_START 23

#define PROGRAM_EXEC_ADDR 0x8048000

#define _8MB	0x800000
#define _8KB	0x2000

#define VID_MEM_START 0x08000000
#define VID_MEM_END   0x08400000

extern int32_t halt (uint8_t status);
extern int32_t execute (const uint8_t * command);
extern int32_t read (int32_t fd, void * buf, int32_t nbytes);
extern int32_t write (int32_t fd, const void * buf, int32_t nbytes);
extern int32_t open (const uint8_t * filename);
extern int32_t close (int32_t fd);
extern int32_t getargs (uint8_t * buf, int32_t nbytes);
extern int32_t vidmap (uint8_t ** screen_start);
extern int32_t set_handler (int32_t signum, void * handler_address);
extern int32_t sigreturn (void);
extern int32_t switch_term(uint8_t dest);
extern int32_t sched(void);
extern int32_t context_switch(uint32_t o_slot, uint32_t n_slot);
extern pcb_t * get_pcb(int32_t term);
extern int32_t first_prog_run();
// extern void * sbrk(uint32_t nbytes);

/* _end is set in the linker command file */
extern void * _end;

#endif /* _SYSCALL_H */
