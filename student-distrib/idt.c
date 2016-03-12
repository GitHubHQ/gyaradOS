#include "idt.h"

// 0 - Division by zero exception
void divide_by_zero_except() {
    printf("EXCEPTION: Divide by zero!\n");
    while(1);
}

// 1 - Debug exception
void debug_except() {
    printf("EXCEPTION: Debug!\n");
    while(1);
}

// 2 - Non maskable interrupt
void nmi_except() {
    printf("EXCEPTION: Non-maskable interrupt!\n");
    while(1);
}

// 3 - Breakpoint exception
void breakpoint_except() {
    printf("EXCEPTION: Breakpoint!\n");
    while(1);
}

// 4 - 'Into detected overflow'
void overflow_except() {
    printf("EXCEPTION: Into detected overflow!\n");
    while(1);
}

// 5 - Out of bounds exception
void oob_except() {
    printf("EXCEPTION: Out of bounds!\n");
    while(1);
}

// 6 - Invalid opcode exception
void opcode_except() {
    printf("EXCEPTION: Invalid opcode!\n");
    while(1);
}

// 7 - No coprocessor exception
void no_coproc_except() {
    printf("EXCEPTION: No coprocessor!\n");
    while(1);
}

// 8 - Double fault (pushes an error code)
void double_fault_except() {
    // TODO push error code

    printf("EXCEPTION: Double fault!\n");
    while(1);
}

// 9 - Coprocessor segment overrun
void coproc_seg_overrun_except() {
    printf("EXCEPTION: Coprocessor segment overrun!\n");
    while(1);
}

// 10 - Bad TSS (pushes an error code)
void tss_except() {
    // TODO push error code

    printf("EXCEPTION: Bad TSS!\n");
    while(1);
}

// 11 - Segment not present (pushes an error code)
void segment_exept() {
    // TODO push error code

    printf("EXCEPTION: Segment not present!\n");
    while(1);
}

// 12 - Stack fault (pushes an error code)
void stack_except() {
    // TODO push error code

    printf("EXCEPTION: Stack fault!\n");
    while(1);
}

// 13 - General protection fault (pushes an error code)
void general_protec_except() {
    // TODO push error code

    printf("EXCEPTION: General protection fault!\n");
    while(1);
}

// 14 - Page fault (pushes an error code)
void page_fault_except() {
    // TODO push error code

    printf("EXCEPTION: Page fault!\n");
    while(1);
}

// 15 - Unknown interrupt exception
void unknown_interr_except() {
    printf("EXCEPTION: Unknown interrupt!\n");
    while(1);
}

// 16 - Coprocessor fault
void coproc_except() {
    printf("EXCEPTION: Coprocessor fault!\n");
    while(1);
}

// 17 - Alignment check exception
void align_except() {
    printf("EXCEPTION: Alignment check!\n");
    while(1);
}

// 18 - Machine check exception
void machine_chk_except() {
    printf("EXCEPTION: Machine check!\n");
}

// keyboard interrupt
void keyboard_interrupt() {
    unsigned long flags;

    cli_and_save(flags);
    printf("%c", kbrd_read_scan_code());
    send_eoi(1);
    restore_flags(flags);
    sti();
}

// rtc interrupt
void rtc_interrupt() {
    unsigned long flags;

    cli_and_save(flags);
    printf("RTC Interrupt!");
    send_eoi(1);
    restore_flags(flags);
    sti();
}

// generic interrupt
void generic_interrupt() {
    unsigned long flags;

    cli_and_save(flags);
    printf("Generic interrupt recieved!\n");
    send_eoi(1);
    restore_flags(flags);
    sti();
}

void init_idt() {
    lidt(idt_desc_ptr);

    int i = 0;
    /// initialize the first 32 vectors as exceptions
    for(i = 0; i < NUM_EXCEPTIONS; i++) {
        // initialize idt properties here

        // present in memory
        idt[i].present = 1;

        // set device priveledge level to be lower
        idt[i].dpl = 0;

        // select the proper segment
        idt[i].seg_selector = KERNEL_CS;

        // exception
        idt[i].size = 1;

        // set up reserved as exception
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved0 = 0;

        // idt[i].offset_15_00;
        // idt[i].offset_31_16;

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

        // set device priveledge level to be lower
        idt[i].dpl = 0;

        // select the proper segment
        idt[i].seg_selector = KERNEL_CS;

        // exception
        idt[i].size = 1;

        // set up reserved as exception
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 0;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved0 = 0;

        // idt[i].offset_15_00;
        // idt[i].offset_31_16;

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
