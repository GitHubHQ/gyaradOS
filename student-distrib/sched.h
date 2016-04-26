#ifndef _SCHED_H
#define _SCHED_H

#include "libs/types.h"
#include "libs/lib.h"

extern pcb_t * concur_proc = {NULL, NULL, NULL};

void sched_init();
void switch_tasks();

#endif /* _SCHED_H */
