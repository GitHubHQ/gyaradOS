
#ifndef _PAGING_H
#define _PAGING_H
#include "libs/types.h"
#include "libs/lib.h"

#define PAGE_DIRECTORY_SIZE     1024
#define PAGE_TABLE_SIZE         1024
#define PAGE_SIZE               4096
#define PAGE_SIZE_LARGE         0x400000

#define VIDEO_PHYS_ADDR         0XB8000
#define KERNEL_PHYS_ADDR        0x400000
#define KERNEL_END_ADDR        0x800000
#define PROCESS_START_ADDR      0x08048000

#define _4KB                    0x1000
#define MAX_PROCESSES           10

typedef union PDE {
        struct {
            uint32_t present : 1;
            uint32_t read_write : 1;
            uint32_t user_super : 1;
            uint32_t write_through : 1;
            uint32_t cache_disabled : 1;
            uint32_t accessed : 1;
            uint32_t reserved : 1;
            uint32_t page_size : 1;
            uint32_t global : 1;
            uint32_t avail : 3;
            uint32_t page_table_base : 20;
        }__attribute__((packed)) PDE_bits;
} PDE_t;
 
typedef union PTE {
    struct {
        uint32_t present : 1;
        uint32_t read_write : 1;
        uint32_t user_super : 1;
        uint32_t write_through : 1;
        uint32_t cache_disabled : 1;
        uint32_t accessed : 1;
        uint32_t dirty : 1;
        uint32_t page_table_attribute : 1;
        uint32_t global : 1;
        uint32_t avail : 3;
        uint32_t page_base : 20;
    }__attribute__((packed)) PTE_bits;   
} PTE_t;

PDE_t pageDirectory[PAGE_DIRECTORY_SIZE] __attribute__((aligned(PAGE_SIZE)));
PTE_t pageTable1[PAGE_TABLE_SIZE] __attribute__((aligned(PAGE_SIZE)));

void init_paging();
int init_new_process(uint32_t process_num);

#endif
