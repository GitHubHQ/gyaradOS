#define ASM 1
#include "x86_desc.h"

.global user_exec
.global jmp_usr_exec

jmp_usr_exec:
    cli
    movl 4(%esp), %ebx
    xorl %eax, %eax
    movw $USER_DS, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl $USER_DS
    pushl $0x083FFFFC #set stack to bottom of 4mb page for this program

    pushfl
    orl $0x200, (%esp)
    pushl $USER_CS
    pushl %ebx
    iret
