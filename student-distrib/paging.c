#include "paging.h"

/* void init_paging()
 * Description: initialize video memory page in 0-4mb range, and kernel memory
 * from 4-8mb
 * Inputs: none
 * Ooutputs: none
 * side effects: none
 */
void init_paging(){

	uint32_t pageAddress = 0;
	int i;
	/*disabling the 4kb pages in the first 4MB of the PD by setting the pages to be
	 * Supervisor privilege, Read/Write, not present
	 */
	for(i = 0; i < PAGE_TABLE_SIZE; i++){
		pageTable1[i] = pageAddress & SET_DEFAULT_MASK; 
		pageAddress += PAGE_SIZE;
    }

   
    //enabling the present and read/write and user bit for the video memory at physical location 0xB8000
	pageTable1[VIDEO_PHYS_ADDR/PAGE_SIZE] |= SET_USER_PRESENT; 
    
    //enabling bits for R/W and Present and supervisor level(0x3)
	pageDirectory[0] = (unsigned int)pageTable1 | SET_PRESENT;

	//enabling bits PS, R/W, Present (10000011)
    pageDirectory[1] = KERNEL_PHYS_ADDR | SET_4MB_PRESENT; 

	for(i = 2; i < PAGE_DIRECTORY_SIZE; i++){
        //setting the rest of the pages to not be present, read/write, and supervisor privilege
		pageDirectory[i] = 0 | SET_OFF; 
	}

	//Enabling paging 
	asm volatile (
		"mov %%eax, %%cr3 				\n" //moving the address of PD to cr3
		"mov %%cr4, %%eax 				\n"
		"or  $0x00000010, %%eax 		\n" //setting PSE in cr4 for 4MB pages
		"mov %%eax, %%cr4				\n"
		"mov %%cr0, %%eax				\n" //enabling the PG flag in CR0
		"or  $0x80000000, %%eax			\n"
		"mov %%eax, %%cr0"				
		: /* no outputs */			
		: "a" (pageDirectory)			
		);						
    
}

int init_new_process(process_num){
      if(process_num > MAX_PROCESSES || process_num < 0){
        return -1;
      }
      //get address and index
      uint32_t address = PROCESS_START_ADDR + process_num*PAGE_SIZE_LARGE;
      uint32_t idx = address/PAGE_SIZE_LARGE;

      //enable the given page;
      pageDirectory[idx] = address | SET_4MB_USER_PRESENT;
      
      return 0;
}

/* void test_paging()
 * Description: Test access to different pages in memory. Purely for testing
 * purposes.
 * Inputs: none
 * Ooutputs: none
 * side effects: none
 */
void test_paging(){

    // test for access at 0x400000
    uint8_t* x = (uint8_t*) KERNEL_PHYS_ADDR;
    printf("\nkernel: %x\n",*x);
    
    // test access from in kernel
    uint8_t* y = (uint8_t*) 0x600000;
    printf("\n4-8:%x\n",*y);

    // test access to 0xB8000
    uint8_t* z = (uint8_t*) VIDEO_PHYS_ADDR;
    printf("\nvidmem: %x\n",*z);

    /* The following lines are to test for page faults */
    /*
    uint8_t* a = (uint8_t*) 0x300000;
    printf("\npost-vidmem: %x\n",*a);

    uint8_t* b = (uint8_t*) 0xA8000;
    printf("\npre-vidmem: %x\n",*b);


    uint8_t* c = (uint8_t*) 0x900000;
    printf("\npost-kernel: %x\n",*c);
    */
}
