#include "pit.h"

void pit_init() {
    // set up the pit to 30hz
    uint32_t divider = calc_divider(30);

    outb(PIT_SQ_MODE, PIT_CMD_PORT);
    outb((divider & DIVIDER_MASK), PIT_DATA_PORT);
    outb((divider >> 8), PIT_DATA_PORT);
}

uint32_t calc_divider(uint32_t msec) {
    return PIT_MAX_DIV / msec;
}

void pit_handle_interrupt() {
    unsigned long flags;

    // Disable interrupts
    cli_and_save(flags);

    printf("PIT Interruption!\n");

    send_eoi(IRQ_PIT);

    // Restore flags
    restore_flags(flags);
}
