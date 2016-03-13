#include "types.h"
#include "paging.h"

/* The page directory */
uint32_t pageDirectory[PAGE_DIRECTORY_SIZE] __attribute__((aligned(PAGE_DIRECTORY_ALIGN)));

uint32_t pageTable1[PAGE_TABLE_SIZE] __attribute__((aligned(PAGE_SIZE)));

void init_paging(){

	uint32_t pageAddress = 0;
	int i;

	//disabling the 4kb pages in the first 4MB of the PD
	for(i = 0; i < PAGE_TABLE_SIZE; i++){
		pageTable1[i] = pageAddress | 2; 
		pageAddress += 4096; //4kb = 4096
	}
	pageTable1[0xB8] |= 3; //enabing the present bit for the video memory at physical location 0xB8000

	pageDirectory[0] = (unsigned int)pageTable1 | 3;
	pageDirectory[1] = KERNEL_PHYS_ADDR | 131; //enabling bits PS, R/W, Present (10000011)

	for(i = 2; i < PAGE_DIRECTORY_SIZE; i++){
		pageDirectory[i] |= 2; //setting the rest of the pages to not be present (10)
	}

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
