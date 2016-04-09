#define ASM 1
#include "x86_desc.h"

.global jmp_usr_exec
.global jmp_kern_halt

jmp_usr_exec:
    cli
    xorl %ebx, %ebx
    xorl %edi, %edi
    xorl %esi, %esi

    movl 4(%esp), %ebx
    movl $USER_DS, %edi
    movl $USER_CS, %esi

    movw %di, %ds
    movw %di, %es
    movw %di, %fs
    movw %di, %gs

    pushl %edi

    pushl $0x83FFFFC #set stack to bottom of 4mb page for this program

    pushfl
    orl $0x200, (%esp)
    pushl %esi
    pushl %ebx

    iret

jmp_kern_halt:
    leave
    ret