#include "pit.h"

#define SCHED_ENABLED   1

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
    // Disable interrupts
    unsigned long flags;
    cli_and_save(flags);

    // Send EOI to end the interrupt
    send_eoi(IRQ_SYSTEM_TIMER);

    if(!SCHED_ENABLED) {
        restore_flags(flags);
        return;
    }

    // if the first program hasnt been run yet, we dont want to schedule anything!
    if(!first_prog_run()) {
        // Restore flags
        restore_flags(flags);
        return;
    }

    // get the current and next processes to run
    uint8_t curr_proc_term_num = get_curr_running_term_proc();
    uint8_t next_proc_term_num = get_next_running_term_proc();

    // if there is only one process running in one terminal, we have nothing to schedule! return
    if(curr_proc_term_num == next_proc_term_num) {
        restore_flags(flags);
        return;
    }

    // get the pcbs for the current and next processes
    pcb_t * curr_proc = get_pcb(curr_proc_term_num);
    pcb_t * next_proc = get_pcb(next_proc_term_num);

    // if something went wrong and either are null, return silently
    if(curr_proc == NULL || next_proc == NULL) {
        restore_flags(flags);
        return;
    }

    // store ksp/kbp before move to the current processes pcb
    asm volatile("movl %%esp, %0":"=g"(curr_proc->p_sched_ksp));
    asm volatile("movl %%ebp, %0":"=g"(curr_proc->p_sched_kbp));

    // set the correct running process
    set_running_proc(next_proc_term_num);

    // change the page directory to the correct process
    switch_pd(next_proc->proc_num, next_proc->base);

    // set the esp0 to the correct one for the next process
    tss.esp0 = _8MB - (_8KB) * (next_proc->proc_num) - 4;

    restore_flags(flags);

    // stack switch
    asm volatile("movl %0, %%esp"::"g"(next_proc->p_sched_ksp));
    asm volatile("movl %0, %%ebp"::"g"(next_proc->p_sched_kbp));

    // go into the next program
    asm volatile("leave");
    asm volatile("ret");
}
