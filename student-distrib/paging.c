#include "types.h"
#include "paging.h"

void init_paging(){

	uint32_t pageDirectory[1024]__attribute__((aligned(4096)));

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
