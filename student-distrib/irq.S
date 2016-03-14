
#define ASM 1

.global key_irq
.global rtc_irq

/*
 * key_irq()
 *
 * Description: Calls the keyboard handler function and then exits the interrupt cleanly using iret
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
key_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call handle_keypress

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret

/*
 * rtc_irq()
 *
 * Description: Calls the rtc handler function and then exits the interrupt cleanly using iret
 * Inputs: none
 * Outputs: none
 * Returns: none
 */
rtc_irq:
    // save caller saved registers
    pushl %eax
    pushl %ecx
    pushl %edx

    // call the function
    call rtc_handle_interrupt

    // restore caller saved registers
    popl %edx
    popl %ecx
    popl %eax

    // return to the interrupt handler
    iret
