#include "types.h"
#include "paging.h"

/* The page directory */
uint32_t pageDirectory[PAGE_DIRECTORY_SIZE] __attribute__((aligned(PAGE_DIRECTORY_ALIGN)));

uint32_t pageTable1[PAGE_TABLE_SIZE] __attribute__((aligned(PAGE_SIZE)));

void init_paging(){



	//Enabing paging 
	asm volatile (
		"mov %%eax, %%cr3 				\n" //moving the address of PD to cr3
		"mov %%cr4, %%eax 				\n"
		"or  0x00000010, %%eax 			\n" //setting PSE in cr4 for 4MB pages
		"mov %%eax, %%cr4				\n"
		"mov %%cr0, %%eax				\n" //enabling the PG flag in CR0
		"or  0x80000000, %%eax			\n"
		"mov %%eax, %%cr0"				
		: /* no outputs */			
		: "a" (pageDirectory)			
		);						

}
