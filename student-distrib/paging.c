#include "types.h"
#include "paging.h"

void init_paging(){

    /* The page directory */
	uint32_t pageDirectory[PAGE_DIRECTORY_SIZE]__attribute__((aligned(PAGE_DIRECTORY_ALIGN)));

	asm volatile (
		"mov %%cr3, %%eax 				\n"
		"mov %%eax, %%cr4 				\n"
		"or  %%eax, 0x00000010 			\n"
		"mov %%cr4, %%eax				\n"
		"mov %%eax, %%cr0				\n"
		"or  %%eax, 0x80000000			\n"
		"mov %%cr0, %%eax"				
		:								
		: "r"(pageDirectory)			
		: "eax");						

}
