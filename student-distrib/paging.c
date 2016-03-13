#include "types.h"
#include "paging.h"

/* The page directory */
uint32_t pageDirectory[PAGE_DIRECTORY_SIZE] __attribute__((aligned(PAGE_SIZE)));

uint32_t pageTable1[PAGE_TABLE_SIZE] __attribute__((aligned(PAGE_SIZE)));

void init_paging(){

	uint32_t pageAddress = 0;
	int i;

	//disabling the 4kb pages in the first 4MB of the PD
	for(i = 0; i < PAGE_TABLE_SIZE; i++){
		pageTable1[i] = pageAddress & SET_DEFAULT_MASK; 
		pageAddress += PAGE_SIZE;
    }
   
    //enabling the present bit for the video memory at physical location 0xB8000
	pageTable1[VIDEO_PHYS_ADDR] |= SET_VIDEO_MASK; 
    
    //enabling bits for R/W and Present (0x3)
	pageDirectory[0] = (unsigned int)pageTable1 | SET_VIDEO_MASK;

	//enabling bits PS, R/W, Present (10000011)
    pageDirectory[1] = KERNEL_PHYS_ADDR | SET_KERNEL_MASK; 

	for(i = 2; i < PAGE_DIRECTORY_SIZE; i++){
        //setting the rest of the pages to not be present (10)
		pageDirectory[i] |= 2; 
	}

	//Enabling paging 
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
