
#ifndef _PAGING_H
#define _PAGING_H
#include "types.h"

#define PAGE_DIRECTORY_ALIGN    4096
#define PAGE_DIRECTORY_SIZE     1024
#define PAGE_TABLE_SIZE         1024
#define PAGE_SIZE               4096

#define KERNEL_START_ADDR       0x400000

/* initialize paging */
void init_paging();

#endif
