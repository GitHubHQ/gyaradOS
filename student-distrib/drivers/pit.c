#include "pit.h"

void pit_init() {
    // set up the pit to 30hz
    uint32_t divider = calc_divider(5);

    outb(PIT_SQ_MODE, PIT_CMD_PORT);
    outb((divider & DIVIDER_MASK), PIT_DATA_PORT);
    outb((divider >> 8), PIT_DATA_PORT);
}

uint32_t calc_divider(uint32_t msec) {
    return PIT_MAX_DIV / msec;
}

void pit_handle_interrupt() {
    // Disable interrupts
    unsigned long flags;
    cli_and_save(flags);

    // Send EOI to end the interrupt
    send_eoi(IRQ_SYSTEM_TIMER);

    // Restore flags
    restore_flags(flags);

    sched();
}
