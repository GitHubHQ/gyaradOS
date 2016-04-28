#include "syscall.h"

pcb_t * curr_proc[] = {NULL, NULL, NULL};
pcb_t * prev_proc[] = {NULL, NULL, NULL};
uint8_t curr_terminal = 0;
uint8_t curr_active_p = 0;

uint32_t curr_proc_id_mask = 0;
uint32_t curr_proc_id = 0;

uint32_t first_program_run = 0;

static func_ptr stdin_ops_table[4] = {NULL, terminal_read, NULL, NULL};
static func_ptr stdout_ops_table[4] = {NULL, NULL, terminal_write, NULL};
static func_ptr rtc_ops_table[4] = {rtc_open, rtc_read, rtc_write, rtc_close};
static func_ptr dir_ops_table[4] = {dir_open, dir_read, dir_write, dir_close};
static func_ptr files_ops_table[4] = {fs_open, fs_read, fs_write, fs_close};

int32_t halt (uint8_t status) {
    pcb_t * proc_ctrl_blk = curr_proc[curr_terminal];

    // get the process number to free
    uint32_t free_proc_num = proc_ctrl_blk->proc_num;
    if(free_proc_num == 0) {
        // restart this process since its the first process
        // we can hardcode this to shell since that is the first process every time
        uint8_t f_init_data[32];
        uint32_t entrypoint = 0;

        // Grab the first 32 bytes of the file to see if it is runnable
        // and find where it starts
        file_array exec_read;
        strcpy((int8_t*)&(exec_read.file_name),(int8_t*) "shell");
        exec_read.file_position = 0;

        if(fs_read(&exec_read, f_init_data, 32) == -1) {
            return -1;
        }

        // Grab the entry point of the application
        entrypoint += (uint32_t)f_init_data[27] << 24;
        entrypoint += (uint32_t)f_init_data[26] << 16;
        entrypoint += (uint32_t)f_init_data[25] << 8;
        entrypoint += (uint32_t)f_init_data[24];

        // jump back to the beginning of the executable
        jmp_usr_exec(entrypoint);
    }

    // set the process to free in the process buffer
    curr_proc_id_mask &= ~(1 << free_proc_num);

    // Close STDIN
    proc_ctrl_blk->fds[0].operations_pointer = NULL;
    proc_ctrl_blk->fds[0].flags = NOT_USE;

    // Close STDOUT
    proc_ctrl_blk->fds[1].operations_pointer = NULL;
    proc_ctrl_blk->fds[1].inode = NULL;
    proc_ctrl_blk->fds[1].flags = NOT_USE;

    // Close any open FDS
    int i;
    for(i = 2; i < 8; i ++) {
        close(i);
    }

    // reset the page entries
    switch_pd(prev_proc[curr_terminal]->proc_num, prev_proc[curr_terminal]->base);
    tss.esp0 = _8MB - (_8KB) * prev_proc[curr_terminal]->proc_num - 4;

    // stack switch
    asm volatile("movl %0, %%esp"::"g"(proc_ctrl_blk->p_ksp));
    asm volatile("movl %0, %%ebp"::"g"(proc_ctrl_blk->p_kbp));

    // swap the pcbs correctly
    curr_proc[curr_terminal] = prev_proc[curr_terminal];
    prev_proc[curr_terminal] = (pcb_t *) prev_proc[curr_terminal]->prev;

    asm volatile("jmp EXECUTE_EXIT");

    return 0;
}

int32_t execute (const uint8_t * command) {
    // Used to hold the first 32 bytes of the file
    // These 32 bytes will contain the exec information and the entry point of the file
    uint8_t f_init_data[32];
    uint32_t entrypoint = 0;
    uint32_t temp_process_mask = PROGRAM_LOCATION_MASK;
    uint32_t curr_proc_id = 0;
    uint32_t i;

    // fail if an invalid command is specified
    if (command == NULL || strlen((int8_t *) command) == 0) {
        return -1;
    }

    // Store the command args
    uint8_t space_flag = 0;
    uint8_t cmd_len;
    uint8_t temp_arg[64];
    for(i = 0; command[i] != '\0' ; i++) {
        if(command[i] == ' ' && space_flag == 0) {
            space_flag = 1;
            cmd_len = i;
        } else if(space_flag == 1) {
            temp_arg[i - cmd_len - 1] = command[i];
        } else {
            if(i >= 32 && space_flag == 0) {
                return -1;
            }
        }
    }

    temp_arg[i-cmd_len-1] = '\0';

    // get the file name to execute
    uint8_t * f_name = strtok(command);

    // Grab the first 32 bytes of the file to see if it is runnable
    // and find where it starts
    file_array exec_read;
    strcpy((int8_t*)&(exec_read.file_name),(int8_t*)f_name);
    exec_read.file_position = 0;
    if(fs_read(&exec_read, f_init_data, 32) == -1) {
        return -1;
    }

    // See if the file is executeable
    if (!((f_init_data[0] == MAGIC_NUM_1) && (f_init_data[1] == MAGIC_NUM_2) && (f_init_data[2] == MAGIC_NUM_3) && (f_init_data[3] == MAGIC_NUM_4))) {
        printf("ERROR: Non-Runnable file!\n");
        return -1;
    }

    // Grab the entry point of the application
    entrypoint += (uint32_t)f_init_data[27] << 24;
    entrypoint += (uint32_t)f_init_data[26] << 16;
    entrypoint += (uint32_t)f_init_data[25] << 8;
    entrypoint += (uint32_t)f_init_data[24];

    // Find a open spot for the program to run
    for (i = 0; i < MAX_PROG_NUM; i++) {
        if(!(temp_process_mask & curr_proc_id_mask)) {
            curr_proc_id_mask |= temp_process_mask;
            curr_proc_id = i;
            break;
        } else {
            temp_process_mask = temp_process_mask >> 1;
        }
    }

    // Max number of programs reached, error out
    if (i >= (MAX_PROG_NUM -1)) {
        printf("ERROR: Out of runnable program slots!\n");
        return -1;
    }

    // Create a page directory for the program
    uint32_t base = init_new_process(curr_proc_id);

    // Copy the program to the page directory
    copy_file_to_addr(f_name, PROGRAM_EXEC_ADDR);

    // Create a process control block for our program in the kernel stack
    pcb_t * proc_ctrl_blk = (pcb_t*) (_8MB - ((_8KB)*(curr_proc_id + 1)));

    // Grab and store the ESP and EBP in the PCB
    asm volatile("movl %%esp, %0":"=g"(proc_ctrl_blk->p_ksp));
    asm volatile("movl %%ebp, %0":"=g"(proc_ctrl_blk->p_kbp));

    // Store Proc ID
    proc_ctrl_blk->proc_num = curr_proc_id;

    // store Prev address
    proc_ctrl_blk->base = base;

    // store arguments into pcb
    strcpy((int8_t*)proc_ctrl_blk->args, (const int8_t*)temp_arg);

    // Initalize PCB file descriptors
    for (i = 0; i < 8; i++) {
        proc_ctrl_blk->fds[i].operations_pointer = NULL;
        proc_ctrl_blk->fds[i].inode = NULL;
        proc_ctrl_blk->fds[i].file_position = 0;
        proc_ctrl_blk->fds[i].flags = NOT_USE;
    }

    // Set TSS Value
    tss.esp0 = _8MB - (_8KB) * curr_proc_id - 4;

    // Open STDIN
    proc_ctrl_blk->fds[0].operations_pointer = stdin_ops_table;
    proc_ctrl_blk->fds[0].flags = IN_USE;

    // Open STDOUT
    proc_ctrl_blk->fds[1].operations_pointer = stdout_ops_table;
    proc_ctrl_blk->fds[1].inode = NULL;
    proc_ctrl_blk->fds[1].flags = IN_USE;

    // set pcbs correctly
    prev_proc[curr_terminal] = curr_proc[curr_terminal];
    curr_proc[curr_terminal] = proc_ctrl_blk;
    curr_proc[curr_terminal]->prev = (struct pcb_t *) prev_proc[curr_terminal];

    first_program_run = 1;

    // jump to the program to begin execution
    jmp_usr_exec(entrypoint);

    asm volatile("EXECUTE_EXIT:");

    return 0;
}

int32_t read (int32_t fd, void * buf, int32_t nbytes) {
    if (buf == NULL || fd > 7 || fd < 0 || fd == 1 || curr_proc[curr_terminal]->fds[fd].flags != IN_USE) {
        return -1;
    }
    return curr_proc[curr_terminal]->fds[fd].operations_pointer[READ](&(curr_proc[curr_terminal]->fds[fd]), buf, nbytes);
}

int32_t write (int32_t fd, const void * buf, int32_t nbytes) {
    if (buf == NULL || fd > 7 || fd <= 0 || curr_proc[curr_terminal]->fds[fd].flags != IN_USE) {
        return -1;
    }
    return curr_proc[curr_terminal]->fds[fd].operations_pointer[WRITE](fd, buf, nbytes);
}

int32_t open (const uint8_t * filename) {
    // fail if an invalid filename is specified
    if (filename == NULL || strlen((int8_t *) filename) == 0) {
        return -1;
    }

    dentry_t file_info;
    int32_t check = read_dentry_by_name(filename, &file_info);

    //checking if the file name exists
    if(check == -1) {
        return -1;
    }

    //put back calling open
    int i = 0;
    for(i = 2; i < MAX_FILES; i++) {
        if(curr_proc[curr_terminal]->fds[i].flags == NOT_USE) {
            switch(file_info.file_type) {
                case RTC_TYPE:
                    curr_proc[curr_terminal]->fds[i].operations_pointer = rtc_ops_table;
                    curr_proc[curr_terminal]->fds[i].inode = NULL;
                    curr_proc[curr_terminal]->fds[i].file_position = 0;
                    strcpy((int8_t*)&(curr_proc[curr_terminal]->fds[i].file_name), (int8_t*) filename);
                    rtc_open();
                    break;
                case DIR_TYPE:
                    curr_proc[curr_terminal]->fds[i].operations_pointer = dir_ops_table;
                    curr_proc[curr_terminal]->fds[i].inode = NULL;
                    curr_proc[curr_terminal]->fds[i].file_position = 0;
                    strcpy((int8_t*)&(curr_proc[curr_terminal]->fds[i].file_name), (int8_t*) filename);
                    dir_open(filename);
                    break;
                case FILE_TYPE:
                    curr_proc[curr_terminal]->fds[i].operations_pointer = files_ops_table;
                    curr_proc[curr_terminal]->fds[i].inode = get_inode(file_info.inode_num);
                    curr_proc[curr_terminal]->fds[i].file_position = 0;
                    strcpy((int8_t*)&(curr_proc[curr_terminal]->fds[i].file_name), (int8_t*) filename);
                    fs_open(filename);
                    break;
            }
            curr_proc[curr_terminal]->fds[i].flags = IN_USE;
            return i;
        }
    }
    return -1;
}

int32_t close (int32_t fd) {
    if(fd >= 2 && fd <= 7 && (curr_proc[curr_terminal]->fds[fd].flags == IN_USE)) {
        curr_proc[curr_terminal]->fds[fd].flags = NOT_USE;
        curr_proc[curr_terminal]->fds[fd].file_position = 0;
        return curr_proc[curr_terminal]->fds[fd].operations_pointer[CLOSE](fd);
    }

    return -1;
}

int32_t getargs (uint8_t * buf, int32_t nbytes) {
    if(nbytes < 0 || buf == NULL) {
        return -1;
    }

    strncpy((int8_t*) buf, (const int8_t*) curr_proc[curr_terminal]->args, nbytes);
    return 0;
}

int32_t vidmap (uint8_t ** screen_start) {
    if((uint32_t) screen_start < VID_MEM_START || (uint32_t) screen_start > VID_MEM_END) {
        return -1;
    }

    *screen_start = (uint8_t *) VIDEO_PHYS_ADDR;

    return 0;
}

int32_t set_handler (int32_t signum, void * handler_address) {
    return 0;
}

int32_t sigreturn (void) {
    return 0;
}

int32_t switch_term(uint8_t dest) {
    // switch terminals to the new one
    curr_terminal = dest;

    return 0;
}

pcb_t * get_pcb(int32_t term) {
    return curr_proc[term];
}

int32_t first_prog_run() {
    return first_program_run;
}

uint8_t get_curr_running_term_proc() {
    return curr_active_p;
}

uint8_t get_next_running_term_proc() {
    uint8_t n_term_num = curr_active_p;
    uint8_t found = 0;

    while(!found) {
        n_term_num++;

        if(n_term_num >= MAX_RUN_PROG) {
            n_term_num = 0;
        }

        if(curr_proc[n_term_num] != NULL) {
            found = 1;
            return n_term_num;
        }
    }
}

void set_running_proc(uint8_t proc) {
    curr_active_p = proc;
    return;
}

// int32_t sched(void) {
//     if(!first_program_run){
//         return -1;
//     }

//     pcb_t * c_running_proc = curr_proc[curr_active_p];
//     pcb_t * n_running_proc = NULL;

//     uint32_t old_proc_num = curr_active_p;

//     while(n_running_proc == NULL) {
//         curr_active_p++;
//         if(curr_active_p >= MAX_RUN_PROG) {
//             curr_active_p = 0;
//         }
//         n_running_proc = curr_proc[curr_active_p];
//     }

//     context_switch(old_proc_num, curr_active_p);

//     return 0;
// }

// int32_t context_switch(uint32_t o_slot, uint32_t n_slot) {
//     pcb_t * o_pcb = get_pcb(o_slot);
//     // Grab PCB for next
//     pcb_t * n_pcb = get_pcb(n_slot);
                        
//     switch_pd(n_pcb->proc_num, n_pcb->base);
//     tss.ss0 = KERNEL_DS;
//     tss.esp0 = _8MB - (_8KB) * (n_pcb->proc_num) - 4;
//     // save
//     asm volatile("movl %%esp, %0":"=r"(o_pcb->ksp));
//     asm volatile("movl %%ebp, %0":"=r"(o_pcb->kbp));

//     // load
//     asm volatile("movl %0, %%esp"::"r"(n_pcb->ksp));
//     asm volatile("movl %0, %%ebp"::"r"(n_pcb->kbp));
    
//     return 0;
// }

// /*
//  * Thanks to: https://sourceware.org/newlib/libc.html#Syscalls
//  *     (Red Hat Minimal Implementation)
//  * And to: http://code.metager.de/source/xref/hurd/viengoos/libhurd-mm/sbrk.c
//  *     (GNU Hurd Implementation)
//  */
// void * sbrk(uint32_t nbytes) {
//     static void * heap_ptr = NULL;
//     void * base;

//     if (heap_ptr == NULL) {
//         heap_ptr = (void *)&_end;
//     }

//     if ((RAMSIZE - heap_ptr) >= 0) {
//         base = heap_ptr;
//         heap_ptr += nbytes;
//         return (base);
//     } else {
//         return ((void *)-1);
//     }
// }
