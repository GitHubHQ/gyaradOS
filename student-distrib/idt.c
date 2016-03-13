#include "idt.h"

/*
 * divide_by_zero_except()
 *
 * Description: Prints the division by zero exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void divide_by_zero_except() {
    printf("\nEXCEPTION: Divide by zero!\n");
    while(1);
}

/*
 * debug_except()
 *
 * Description: Prints the debug exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void debug_except() {
    printf("\nEXCEPTION: Debug!\n");
    while(1);
}

/*
 * nmi_except()
 *
 * Description: Prints the NMI exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void nmi_except() {
    printf("\nEXCEPTION: Non-maskable interrupt!\n");
    while(1);
}

/*
 * breakpoint_except()
 *
 * Description: Prints the breakpoint exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void breakpoint_except() {
    printf("\nEXCEPTION: Breakpoint!\n");
    while(1);
}

/*
 * overflow_except()
 *
 * Description: Prints the overflow exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void overflow_except() {
    printf("\nEXCEPTION: Into detected overflow!\n");
    while(1);
}

/*
 * oob_except()
 *
 * Description: Prints the out of bounds exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void oob_except() {
    printf("\nEXCEPTION: Out of bounds!\n");
    while(1);
}

/*
 * opcode_except()
 *
 * Description: Prints the opcode exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void opcode_except() {
    printf("\nEXCEPTION: Invalid opcode!\n");
    while(1);
}

/*
 * no_coproc_except()
 *
 * Description: Prints the no coprocessor exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void no_coproc_except() {
    printf("\nEXCEPTION: No coprocessor!\n");
    while(1);
}

/*
 * double_fault_except()
 *
 * Description: Prints the double fault exception when detected along with relevant error
 *              code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void double_fault_except() {
    uint32_t saved_eip;
    asm("\t movl 4(%%esp),%0" : "=r"(saved_eip));

    uint32_t saved_cs;
    asm("\t movl 8(%%esp),%0" : "=r"(saved_cs));

    uint32_t prev_stack;
    asm("\t popl %0" : "=r"(prev_stack));

    printf("\nEXCEPTION: Double fault!\n");
    printf("Faulty instruction at: %x\n", saved_eip);
    printf("Code segment: %d\n", saved_cs);
    printf("Error code: %d", prev_stack);

    while(1);
}

/*
 * coproc_seg_overrun_except()
 *
 * Description: Prints the coprocessor segment overrun exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void coproc_seg_overrun_except() {
    printf("\nEXCEPTION: Coprocessor segment overrun!\n");
    while(1);
}

/*
 * tss_except()
 *
 * Description: Prints the tss exception when detected along with relevant error code data,
 *              and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void tss_except() {
    uint32_t saved_eip;
    asm("\t movl 4(%%esp),%0" : "=r"(saved_eip));

    uint32_t saved_cs;
    asm("\t movl 8(%%esp),%0" : "=r"(saved_cs));

    uint32_t prev_stack;
    asm("\t popl %0" : "=r"(prev_stack));

    printf("\nEXCEPTION: Bad TSS!\n");
    printf("Faulty instruction at: %x\n", saved_eip);
    printf("Code segment: %d\n", saved_cs);
    printf("Error code: %d", prev_stack);

    while(1);
}

/*
 * segment_exept()
 *
 * Description: Prints the segment not present exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void segment_exept() {
    uint32_t saved_eip;
    asm("\t movl 4(%%esp),%0" : "=r"(saved_eip));

    uint32_t saved_cs;
    asm("\t movl 8(%%esp),%0" : "=r"(saved_cs));

    uint32_t prev_stack;
    asm("\t popl %0" : "=r"(prev_stack));

    printf("\nEXCEPTION: Segment not present!\n");
    printf("Faulty instruction at: %x\n", saved_eip);
    printf("Code segment: %d\n", saved_cs);
    printf("Error code: %d", prev_stack);

    while(1);
}

/*
 * stack_except()
 *
 * Description: Prints the stack fault exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void stack_except() {
    uint32_t saved_eip;
    asm("\t movl 4(%%esp),%0" : "=r"(saved_eip));

    uint32_t saved_cs;
    asm("\t movl 8(%%esp),%0" : "=r"(saved_cs));

    uint32_t prev_stack;
    asm("\t popl %0" : "=r"(prev_stack));

    printf("\nEXCEPTION: Stack fault!\n");
    printf("Faulty instruction at: %x\n", saved_eip);
    printf("Code segment: %d\n", saved_cs);
    printf("Error code: %d", prev_stack);

    while(1);
}

/*
 * general_protec_except()
 *
 * Description: Prints the general protection exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void general_protec_except() {
    uint32_t saved_eip;
    asm("\t movl 4(%%esp),%0" : "=r"(saved_eip));

    uint32_t saved_cs;
    asm("\t movl 8(%%esp),%0" : "=r"(saved_cs));

    uint32_t prev_stack;
    asm("\t popl %0" : "=r"(prev_stack));

    printf("\nEXCEPTION: General protection fault!\n");
    printf("Faulty instruction at: %x\n", saved_eip);
    printf("Code segment: %d\n", saved_cs);
    printf("Error code: %d", prev_stack);
    
    while(1);
}

/*
 * page_fault_except()
 *
 * Description: Prints the page fault exception when detected along with
 *              relevant error code data, and then spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void page_fault_except() {
    uint32_t saved_eip;
    asm("\t movl 4(%%esp),%0" : "=r"(saved_eip));

    uint32_t saved_cs;
    asm("\t movl 8(%%esp),%0" : "=r"(saved_cs));

    uint32_t prev_stack;
    asm("\t popl %0" : "=r"(prev_stack));

    printf("\nEXCEPTION: Page fault!\n");
    printf("Faulty instruction at: %x\n", saved_eip);
    printf("Code segment: %d\n", saved_cs);
    printf("Error code: %d", prev_stack);
    while(1);
}

/*
 * unknown_interr_except()
 *
 * Description: Prints the unknown interrupt exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void unknown_interr_except() {
    printf("\nEXCEPTION: Unknown interrupt!\n");
    while(1);
}

/*
 * coproc_except()
 *
 * Description: Prints the coprocessor fault exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void coproc_except() {
    printf("\nEXCEPTION: Coprocessor fault!\n");
    while(1);
}

/*
 * align_except()
 *
 * Description: Prints the alignment check exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void align_except() {
    printf("\nEXCEPTION: Alignment check!\n");
    while(1);
}

/*
 * machine_chk_except()
 *
 * Description: Prints the machine check exception when detected and spins infinitely
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void machine_chk_except() {
    printf("\nEXCEPTION: Machine check!\n");
    while(1);
}

/*
 * keyboard_interrupt()
 *
 * Description: Handles the keyboard interrupt by printing the appropriate 
 *              keycode to console and then sending the eoi signal
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void keyboard_interrupt() {
    cli();
    kbrd_print_keypress();
    send_eoi(1);
    sti();
}

/*
 * rtc_interrupt()
 *
 * Description: Handles the rtc interrupt by printing that fact and then sending the eoi signal
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void rtc_interrupt() {
    cli();
    printf("RTC Interrupt!\n");
    send_eoi(IRQ_RTC);
    rtc_special_eoi();
    sti();
}

/*
 * generic_interrupt()
 *
 * Description: Handles any other generic interrupts that don't currently have specific handlers
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void generic_interrupt() {
    cli();
    printf("\nGeneric interrupt recieved!\n");
    send_eoi(1);
    sti();
}

/* init_idt()
 *
 * Description: Initializes the interrupt descriptor table with both exceptions and interrupts.
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void init_idt() {
    lidt(idt_desc_ptr);

    int i = 0;
    /// initialize the first 32 vectors as exceptions
    for(i = 0; i < NUM_EXCEPTIONS; i++) {
        // initialize idt properties here
        // present in memory
        idt[i].present = 1;

        // set device priveledge level to be kernel
        idt[i].dpl = 0;

        // select the proper segment
        idt[i].seg_selector = KERNEL_CS;

        // set this as an exception
        idt[i].size = 1;

        // set up reserved as exception
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved0 = 0;

        // assign the proper idt entry with exception
        switch(i) {
            case 0:
                SET_IDT_ENTRY(idt[i], divide_by_zero_except);
                break;
            case 1:
                SET_IDT_ENTRY(idt[i], debug_except);
                break;
            case 2:
                SET_IDT_ENTRY(idt[i], nmi_except);
                break;
            case 3:
                SET_IDT_ENTRY(idt[i], breakpoint_except);
                break;
            case 4:
                SET_IDT_ENTRY(idt[i], overflow_except);
                break;
            case 5:
                SET_IDT_ENTRY(idt[i], oob_except);
                break;
            case 6:
                SET_IDT_ENTRY(idt[i], opcode_except);
                break;
            case 7:
                SET_IDT_ENTRY(idt[i], no_coproc_except);
                break;
            case 8:
                SET_IDT_ENTRY(idt[i], double_fault_except);
                break;
            case 9:
                SET_IDT_ENTRY(idt[i], coproc_seg_overrun_except);
                break;
            case 10:
                SET_IDT_ENTRY(idt[i], tss_except);
                break;
            case 11:
                SET_IDT_ENTRY(idt[i], segment_exept);
                break;
            case 12:
                SET_IDT_ENTRY(idt[i], stack_except);
                break;
            case 13:
                SET_IDT_ENTRY(idt[i], general_protec_except);
                break;
            case 14:
                SET_IDT_ENTRY(idt[i], page_fault_except);
                break;
            case 15:
                SET_IDT_ENTRY(idt[i], unknown_interr_except);
                break;
            case 16:
                SET_IDT_ENTRY(idt[i], coproc_except);
                break;
            case 17:
                SET_IDT_ENTRY(idt[i], align_except);
                break;
            case 18:
                SET_IDT_ENTRY(idt[i], machine_chk_except);
                break;
            default:
                // reserved, don't set the entry
                break;
        }
    }

    // initialize the rest of the vectors as interrupts
    for(i = NUM_EXCEPTIONS; i < NUM_VEC; i++) {
        // initialize idt properties here
        // present in memory
        idt[i].present = 1;

        // set device privilege level to be kernel
        idt[i].dpl = 0;

        // select the proper segment
        idt[i].seg_selector = KERNEL_CS;

        // set this as interrupts
        idt[i].size = 0;

        // set up reserved as exception
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 0;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved0 = 0;

        switch(i) {
            case KEYBOARD_IDT:
                SET_IDT_ENTRY(idt[i], keyboard_interrupt);
                break;
            case RTC_IDT:
                SET_IDT_ENTRY(idt[i], rtc_interrupt);
                break;
            default:
                SET_IDT_ENTRY(idt[i], generic_interrupt);
                break;
        }
    }
}
