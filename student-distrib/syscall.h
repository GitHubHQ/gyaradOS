
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

#define MAX_NUMBER_ARGS	  32

#define OPEN  			   0
#define READ  			   1
#define WRITE  			   2
#define CLOSE  			   3

#define IN_USE			   1
#define NOT_USE			   0

#define MAX_FILES		   8

#define PROGRAM_LOCATION_MASK	0x80000000
#define MAX_PROG_NUM	32

#define NUM_BYTES_STATS   28
#define ENTRY_POINT_START 23

#define PROGRAM_EXEC_ADDR 0x8048000


#define _8MB	0x800000
#define _8KB	0x2000

typedef struct {
    uint32_t * operations_pointer;
    inode_t * inode;
    uint32_t file_position;
    uint32_t flags;
} file_array;

typedef struct pcb{
    file_array fds[8];
    uint8_t file_names[8][32];
    uint32_t ksp;
    uint32_t kbp;
    uint32_t p_ksp;
    uint32_t p_kbp;
    uint8_t proc_num;
    uint8_t p_proc_num;
    uint8_t children;
} pcb_t;

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

#endif /* _SYSCALL_H */
