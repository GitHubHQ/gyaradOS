#include "syscall.h"

pcb_t * curr_proc = NULL;
pcb_t * prev_proc = NULL;

uint32_t curr_proc_id_mask = 0;
uint32_t curr_proc_id = 0;

uint32_t* stdin_ops_table[4] = {NULL, (uint32_t *) terminal_read, NULL, NULL};
uint32_t* stdout_ops_table[4] = {NULL, NULL, (uint32_t *) terminal_write, NULL};
uint32_t* rtc_ops_table[4] = {(uint32_t *) rtc_open, (uint32_t *) rtc_read, (uint32_t *) rtc_write, (uint32_t *) rtc_close};
uint32_t* dir_ops_table[4] = {(uint32_t *) dir_open, (uint32_t *) dir_read, (uint32_t *) dir_write, (uint32_t *) dir_close};
uint32_t* files_ops_table[4] = {(uint32_t *) fs_open, (uint32_t *) fs_read, (uint32_t *) fs_write, (uint32_t *) fs_close};

uint32_t files_in_use = 2;

int32_t halt (uint8_t status) {
    pcb_t * proc_ctrl_blk = curr_proc;

    // get the process number to free
    uint32_t free_proc_num = proc_ctrl_blk->proc_num;
    if(free_proc_num == 0) {
        // restart this process since its the first process
        // we can hardcode this to shell since that is the first process every time
        uint8_t f_init_data[32];
        uint32_t entrypoint = 0;

        // Grab the first 32 bytes of the file to see if it is runnable
        // and find where it starts
        if(fs_read(((int32_t) "shell"), &f_init_data, 32) == -1) {
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

    // reset the page entries
    switch_pd(prev_proc->proc_num, prev_proc->base);
    tss.esp0 = _8MB - (_8KB) * prev_proc->proc_num - 4;

    // stack switch
    asm volatile("movl %0, %%esp"::"g"(proc_ctrl_blk->p_ksp));
    asm volatile("movl %0, %%ebp"::"g"(proc_ctrl_blk->p_kbp));

    // swap the pcbs correctly
    curr_proc = prev_proc;
    prev_proc = prev_proc->prev;

    asm volatile("jmp HELLO");

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
    if (command == NULL || strlen(command) == 0) {
        return -1;
    }

    // get the file name to execute
    uint8_t * f_name = strtok(command);

    // Grab the first 32 bytes of the file to see if it is runnable
    // and find where it starts
    if(fs_read((int32_t)f_name, &f_init_data, 32) == -1) {
        return -1;
    }

    // See if the file is executeable
    if (!((f_init_data[0] == MAGIC_NUM_1) && (f_init_data[1] == MAGIC_NUM_2) && (f_init_data[2] == MAGIC_NUM_3) && (f_init_data[3] == MAGIC_NUM_4))) {
        printf("%s\n", "Non-Runnable file!");
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
    if (i == (MAX_PROG_NUM -1)) {
        return -1;
    }

    // Create a page directory for the program
    uint32_t base = init_new_process(curr_proc_id);

    // Copy the program to the page directory
    copy_file_to_addr(f_name, PROGRAM_EXEC_ADDR);

    // Create a process control block for our program in the kernel stack
    pcb_t * proc_ctrl_blk = (pcb_t*) (_8MB - (_8KB)*(curr_proc_id + 1));

    // Grab and store the ESP and EBP in the PCB
    asm volatile("movl %%esp, %0":"=g"(proc_ctrl_blk->p_ksp));
    asm volatile("movl %%ebp, %0":"=g"(proc_ctrl_blk->p_kbp));

    // Store Proc ID
    proc_ctrl_blk->proc_num = curr_proc_id;

    // store Prev address
    proc_ctrl_blk->base = base;

    // Initalize PCB file descriptors
    for (i = 0; i < 8; ++i) {
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

    prev_proc = curr_proc;
    curr_proc = proc_ctrl_blk;

    curr_proc->prev = prev_proc;

    jmp_usr_exec(entrypoint);

    asm volatile("HELLO:");

    return 0;
}

int32_t read (int32_t fd, void * buf, int32_t nbytes) {
    int32_t (*func_ptr)(int32_t fd, void * buf, int32_t nbytes);
    func_ptr = curr_proc->fds[fd].operations_pointer[READ];
    if(func_ptr == NULL)
        return -1;
    else
        return func_ptr(fd, buf, nbytes);
}

int32_t write (int32_t fd, const void * buf, int32_t nbytes) {
    int32_t (*func_ptr)(int32_t fd, void * buf, int32_t nbytes);
    func_ptr = curr_proc->fds[fd].operations_pointer[WRITE];
    if(func_ptr == NULL)
        return -1;
    else
        return func_ptr(fd, buf, nbytes);
}

int32_t open (const uint8_t * filename) {
    dentry_t file_info;
    int32_t check = read_dentry_by_name(filename, &file_info);

    //checking if the file name exists
    if(check == -1 || files_in_use > MAX_FILES) {
        return -1;
    }

    //put back calling open
    int i = 0;
    for(i = 2; i < MAX_FILES; i++) {
        if(curr_proc->fds[i].flags == NOT_USE) {
            switch(file_info.file_type) {
                case 0:
                    curr_proc->fds[i].operations_pointer = (uint32_t *) rtc_ops_table;
                    curr_proc->fds[i].inode = NULL;
                    curr_proc->fds[i].file_position = 0;
                    rtc_open();
                    break;
                case 1:
                    curr_proc->fds[i].operations_pointer = (uint32_t *) dir_ops_table;
                    curr_proc->fds[i].inode = NULL;
                    curr_proc->fds[i].file_position = 0;
                    dir_open(filename);
                    break;
                case 2:
                    curr_proc->fds[i].operations_pointer = (uint32_t *) files_ops_table;
                    curr_proc->fds[i].inode = get_inode(file_info.inode_num);
                    curr_proc->fds[i].file_position = 0;
                    fs_open(filename);
                    break;
            }

            curr_proc->fds[i].flags = IN_USE;
            files_in_use++;
            return i;
        }
    }


    return -1;
}

int32_t close (int32_t fd) {
    if(fd >= 2 && fd <= 7) {
        curr_proc->fds[fd].flags = NOT_USE;
        files_in_use--;
        int32_t (*func_ptr)(void);
        func_ptr = curr_proc->fds[fd].operations_pointer[CLOSE];
        return 0;
    }

    return -1;
}

int32_t getargs (uint8_t * buf, int32_t nbytes) {
    return -1;
}

int32_t vidmap (uint8_t ** screen_start) {
    if((uint32_t) screen_start < VID_MEM_START || (uint32_t) screen_start > VID_MEM_END) {
        return -1;
    }

    *screen_start = (uint8_t *) VIDEO;
    return 0;
}

int32_t set_handler (int32_t signum, void * handler_address) {
    return -1;
}

int32_t sigreturn (void) {
    return -1;
}
