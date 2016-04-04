#define ASM 1
#include "x86_desc.h"

.global user_exec
.global jmp_usr_exec

user_exec:
    # set up the new segment selectors to jump to userspace
    xorl %ecx, %ecx
    movl $USER_CS, %ecx
    movw %cx, %cs

    xorl %ecx, %ecx
    movl $USER_DS, %ecx
    movw %cx, %cs
    movw %cx, %es
    movw %cx, %fs
    movw %cx, %gs
    movw %cx, %ss

    xorl %ecx, %ecx
    movl 4(%esp), %ecx
    pushl %ecx

    iret

jmp_usr_exec:
    cli
    movl 4(%esp), %ebx
    xorl %ecx, %ecx
    movl $USER_DS, %ecx
    movw %cx, %ds
    movw %cx, %es
    movw %cx, %fs
    movw %cx, %gs

    pushl %ecx
    pushl $0x83FFFF0 #set stack to bottom of 4mb page for this program

    pushf
    popl %eax
    orl $0x200, %eax
    pushl %eax
    pushl %ecx
    pushl %ebx
    iret
