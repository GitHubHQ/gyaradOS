#define ASM 1
#include "x86_desc.h"

.global user_exec

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