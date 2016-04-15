#include "syscall.h"

pcb_t * curr_proc = NULL;
pcb_t * prev_proc = NULL;

uint32_t curr_proc_id_mask = 0;
uint32_t curr_proc_id = 0;

func_ptr stdin_ops_table[4] = {NULL, terminal_read, NULL, NULL};
func_ptr stdout_ops_table[4] = {NULL, NULL, terminal_write, NULL};
func_ptr rtc_ops_table[4] = { rtc_open,  rtc_read,  rtc_write,  rtc_close};
func_ptr dir_ops_table[4] = { dir_open,  dir_read,  dir_write,  dir_close};
func_ptr files_ops_table[4] = { fs_open,  fs_read,  fs_write,  fs_close};


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
        if(fs_read(((int32_t) "shell"), f_init_data, 32) == -1) {
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
    prev_proc = (pcb_t *) prev_proc->prev;

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
    for( i = 0; command[i] != '\0' ; i++ ) {
        if( command[i] == ' ' && space_flag == 0 ) {
            space_flag = 1;
            cmd_len = i;
            //fname[i] = '\0';
        } else if( space_flag == 1 ) {
            temp_arg[i-cmd_len-1] = command[i];
        } else {
            if(i>=32 && space_flag == 0) {
                return -1;
            }
            //fname[i] = command[i];
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

    
    strcpy((int8_t*)proc_ctrl_blk->args, (const int8_t*)temp_arg);

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

    curr_proc->prev = (struct pcb_t *) prev_proc;

    jmp_usr_exec(entrypoint);

    asm volatile("EXECUTE_EXIT:");

    return 0;
}

int32_t read (int32_t fd, void * buf, int32_t nbytes) {
    int32_t b_return = curr_proc->fds[fd].operations_pointer[READ](&(curr_proc->fds[fd]), buf, nbytes);
    printf("FPOS BEFORE: %d\n", curr_proc->fds[fd].file_position);
    curr_proc->fds[fd].file_position = b_return;
    printf("FPOS: %d\n", curr_proc->fds[fd].file_position);
    printf("BRET %d\n", b_return);
    return b_return;
}

int32_t write (int32_t fd, const void * buf, int32_t nbytes) {
    return curr_proc->fds[fd].operations_pointer[WRITE](fd, buf, nbytes);
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
                    curr_proc->fds[i].operations_pointer = rtc_ops_table;
                    curr_proc->fds[i].inode = NULL;
                    curr_proc->fds[i].file_position = 0;
                    strcpy((int8_t*)&(curr_proc->fds[i].file_name),(int8_t*)filename);
                    rtc_open();
                    break;
                case 1:
                    curr_proc->fds[i].operations_pointer = dir_ops_table;
                    curr_proc->fds[i].inode = NULL;
                    curr_proc->fds[i].file_position = 0;
                    strcpy((int8_t*)&(curr_proc->fds[i].file_name),(int8_t*)filename);
                    dir_open(filename);
                    break;
                case 2:
                    curr_proc->fds[i].operations_pointer = files_ops_table;
                    curr_proc->fds[i].inode = get_inode(file_info.inode_num);
                    curr_proc->fds[i].file_position = 0;
                    strcpy((int8_t*)&(curr_proc->fds[i].file_name),(int8_t*)filename);
                    fs_open(filename);
                    break;
            }

            curr_proc->fds[i].flags = IN_USE;
            files_in_use++;
            return i;
        }
    }
    return 0;
}

int32_t close (int32_t fd) {
    if(fd >= 2 && fd <= 7) {
        curr_proc->fds[fd].flags = NOT_USE;
        curr_proc->fds[fd].file_position = 0;
        files_in_use--;
        curr_proc->fds[fd].operations_pointer[CLOSE](fd);
        return 0;
    }

    return -1;
}

int32_t getargs (uint8_t * buf, int32_t nbytes) {
    strcpy((int8_t*)buf, (const int8_t*)curr_proc->args);
    return 0;
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

/*
 * Thanks to: https://sourceware.org/newlib/libc.html#Syscalls
 *     (Red Hat Minimal Implementation)
 * And to: http://code.metager.de/source/xref/hurd/viengoos/libhurd-mm/sbrk.c
 *     (GNU Hurd Implementation)
 */
void * sbrk(uint32_t nbytes) {
  static void * heap_ptr = NULL;
  void *        base;

  if (heap_ptr == NULL) {
    heap_ptr = (void *)&_end;
  }

  if ((RAMSIZE - heap_ptr) >= 0) {
    base = heap_ptr;
    heap_ptr += nbytes;
    return (base);
  } else {
    return ((void *)-1);
  }
}
