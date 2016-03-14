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
    printf("\nEXCEPTION: Double fault!\n");
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
    printf("\nEXCEPTION: Bad TSS!\n");
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
    printf("\nEXCEPTION: Segment not present!\n");
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
    printf("\nEXCEPTION: Stack fault!\n");
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
    printf("\nEXCEPTION: General protection fault!\n");
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
    printf("\nEXCEPTION: Page fault!\n");
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

void rtc_interrupt() {
    cli();
    printf("RTC Interrupt!\n");
    i8259_ioctl(I8259_SEND_EOI, IRQ_RTC);
    rtc_ioctl(RTC_EOI, 0);
    sti();
}

/*
 * general_interrupt()
 *
 * Description: Handles any other general interrupts that don't currently have specific handlers
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
void general_interrupt() {
    cli();
    printf("General interrupt recieved!\n");
    i8259_ioctl(I8259_SEND_EOI, IRQ_SYSTEM_TIMER);
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
    }

    // set the correct idt entries based off x86 convention
    SET_IDT_ENTRY(idt[0], divide_by_zero_except);
    SET_IDT_ENTRY(idt[1], debug_except);
    SET_IDT_ENTRY(idt[2], nmi_except);
    SET_IDT_ENTRY(idt[3], breakpoint_except);
    SET_IDT_ENTRY(idt[4], overflow_except);
    SET_IDT_ENTRY(idt[5], oob_except);
    SET_IDT_ENTRY(idt[6], opcode_except);
    SET_IDT_ENTRY(idt[7], no_coproc_except);
    SET_IDT_ENTRY(idt[8], double_fault_except);
    SET_IDT_ENTRY(idt[9], coproc_seg_overrun_except);
    SET_IDT_ENTRY(idt[10], tss_except);
    SET_IDT_ENTRY(idt[11], segment_exept);
    SET_IDT_ENTRY(idt[12], stack_except);
    SET_IDT_ENTRY(idt[13], general_protec_except);
    SET_IDT_ENTRY(idt[14], page_fault_except);
    SET_IDT_ENTRY(idt[15], unknown_interr_except);
    SET_IDT_ENTRY(idt[16], coproc_except);
    SET_IDT_ENTRY(idt[17], align_except);
    SET_IDT_ENTRY(idt[18], machine_chk_except);

    // initialize the rest of the vectors as interrupts
    for(i = NUM_EXCEPTIONS; i < NUM_VEC; i++) {
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
        idt[i].reserved3 = 0;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved0 = 0;

        if(i == KEYBOARD_IDT) {
            SET_IDT_ENTRY(idt[KEYBOARD_IDT], key_irq);
        } else if(i == RTC_IDT) {
            SET_IDT_ENTRY(idt[RTC_IDT], rtc_irq);
        } else {
            SET_IDT_ENTRY(idt[i], general_irq);
        }
    }
}
