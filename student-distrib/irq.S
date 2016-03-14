
#define ASM 1

.global key_irq
.global rtc_irq

key_irq:
    pushl %eax
    pushl %ecx
    pushl %edx
    call handle_keypress
    popl %edx
    popl %ecx
    popl %eax
    iret

rtc_irq:
    pushl %eax
    pushl %ecx
    pushl %edx
    call rtc_interrupt
    popl %edx
    popl %ecx
    popl %eax
    iret
