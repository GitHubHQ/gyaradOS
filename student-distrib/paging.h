
#ifndef _PAGING_H
#define _PAGING_H
#include "types.h"

#define PAGE_DIRECTORY_SIZE     1024
#define PAGE_TABLE_SIZE         1024
#define PAGE_SIZE               4096
#define PAGE_SIZE_LARGE         0x400000

#define VIDEO_PHYS_ADDR         0XB8000
#define KERNEL_PHYS_ADDR        0x400000

#define SET_VIDEO_MASK          0x3
#define SET_KERNEL_MASK         0x83
#define SET_READ                0x2
#define SET_DEFAULT_MASK        0xFFFFF000
/* initialize paging */
void init_paging();

#endif
