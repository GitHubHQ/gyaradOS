
#ifndef _PAGING_H
#define _PAGING_H
#include "types.h"

#define PAGE_DIRECTORY_SIZE     1024
#define PAGE_TABLE_SIZE         1024
#define PAGE_SIZE               4096
#define PAGE_SIZE_LARGE         0x400000

#define VIDEO_PHYS_ADDR         0XB8000
#define KERNEL_PHYS_ADDR        0x400000

#define SET_USER_PRESENT        0x7
#define SET_PRESENT 			0x3
#define SET_4MB_PRESENT         0x83
#define SET_OFF                 0x2
#define SET_DEFAULT_MASK        0xFFFFF000
/* The page directory */
uint32_t pageDirectory[PAGE_DIRECTORY_SIZE] __attribute__((aligned(PAGE_SIZE)));

uint32_t pageTable1[PAGE_TABLE_SIZE] __attribute__((aligned(PAGE_SIZE)));

/* initialize paging */
void init_paging(void);

/* test paging */
void test_paging(void);
#endif
