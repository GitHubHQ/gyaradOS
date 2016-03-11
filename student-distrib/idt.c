#include "x86_desc.h"
#include "lib.h"

// 0 - Division by zero exception
// 1 - Debug exception
// 2 - Non maskable interrupt
// 3 - Breakpoint exception
// 4 - 'Into detected overflow'
// 5 - Out of bounds exception
// 6 - Invalid opcode exception
// 7 - No coprocessor exception
// 8 - Double fault (pushes an error code)
// 9 - Coprocessor segment overrun
// 10 - Bad TSS (pushes an error code)
// 11 - Segment not present (pushes an error code)
// 12 - Stack fault (pushes an error code)
// 13 - General protection fault (pushes an error code)
// 14 - Page fault (pushes an error code)
// 15 - Unknown interrupt exception
// 16 - Coprocessor fault
// 17 - Alignment check exception
// 18 - Machine check exception
// 19-31 - Reserved

void init_idt() {
    lidt(idt_desc_ptr);

    int i = 0;
    /// initialize the first 32 vectors as exceptions
    for(i = 0; i < 32; i++) {
        // TODO initialize idt properties here

        // assign the proper idt entry with exception
        switch(i) {
            case 0:
                SET_IDT_ENTRY(idt[i], exception_DE);
                break;
            default:
                break;

        }
    }

    // initialize the rest of the vectors as interrupts
    for(i = 32; i < NUM_VEC; i++) {
        // TODO initialize idt properties here

    }
}