
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "libs/types.h"
#include "fs/files.h"

// null-terminating character for strings
#define NULL_CHAR       '\0'

// magic numbers signifying it is an ELF executable
#define NUM_MAGIC_NUMS     4
#define MAGIC_NUM_1     0x7f
#define MAGIC_NUM_2     0x45
#define MAGIC_NUM_3     0x4c
#define MAGIC_NUM_4     0x46

#define MAX_NUMBER_ARGS	32

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
