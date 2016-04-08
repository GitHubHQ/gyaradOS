#include "syscall.h"

uint8_t * args;
uint32_t index = 0;
uint32_t curr_proc_id_mask = 0;
uint32_t curr_proc_id = 0;
pcb_t* curr_proc = NULL;

file_array files_array[MAX_FILES];

uint32_t* stdin_ops_table[4] = {NULL, (uint32_t *) terminal_read, NULL, NULL};
uint32_t* stdout_ops_table[4] = {NULL, NULL, (uint32_t *) terminal_write, NULL};
uint32_t* rtc_ops_table[4] = {(uint32_t *) rtc_open, (uint32_t *) rtc_read, (uint32_t *) rtc_write, (uint32_t *) rtc_close};
uint32_t* dir_ops_table[4] = {(uint32_t *) dir_open, (uint32_t *) dir_read, (uint32_t *) dir_write, (uint32_t *) dir_close};
uint32_t* files_ops_table[4] = {(uint32_t *) fs_open, (uint32_t *) fs_read, (uint32_t *) fs_write, (uint32_t *) fs_close};


uint32_t files_in_use = 2;

int32_t halt (uint8_t status) {
    return -1;
}


int32_t execute (const uint8_t * command) {

    /* Used to hold the first 32 bytes of the file
        These 32 bytes will contain the exec information
        and the entry point of the file
    */
    uint8_t f_init_data[32];
    uint32_t entrypoint = 0;
    uint32_t temp_process_mask = PROGRAM_LOCATION_MASK;
    uint32_t curr_proc_id = 0;
    uint32_t i;

    if (command == NULL) {
        return -1;
    }
    uint8_t * f_name = simple_strtok(command);
    printf("DEBUG (PROGRAM NAME): %s\n", f_name);

    // Grab the first 32 bytes of the file to see if it is runnable
    // and find where it starts
    if(fs_read((int32_t)f_name, &f_init_data, 32) == -1) {
        return -1;
        printf("%s\n", "FS_READ FAILED!");
    }
    printf("%s\n", "FS_READ PASSED");

    // See if the file is executeable
    printf("First 4 bytes: ");
    printf("0x%x 0x%x 0x%x 0x%x\n", f_init_data[0], f_init_data[1], f_init_data[2], f_init_data[3]);
    if (!((f_init_data[0] == MAGIC_NUM_1) && (f_init_data[1] == MAGIC_NUM_2) && (f_init_data[2] == MAGIC_NUM_3) && (f_init_data[3] == MAGIC_NUM_4))) {
        return -1;
        printf("%s\n", "Non-Runnable file!");
    }
    printf("%s\n", "Runnable file!");

    // Grab the entry point of the application
    entrypoint += (uint32_t)f_init_data[27] << 24;
    entrypoint += (uint32_t)f_init_data[26] << 16;
    entrypoint += (uint32_t)f_init_data[25] << 8;
    entrypoint += (uint32_t)f_init_data[24];
    printf("0x%x\n", entrypoint);

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

    printf("Process ID found: %d\n", curr_proc_id);
    
    // Max number of programs reached, error out
    if (i == (MAX_PROG_NUM -1)) {
        return -1;
    }

    // Create a page directory for the program
    init_new_process(curr_proc_id);
    printf("Paging done?\n");

    // Copy the program to the page directory
    copy_file_to_addr(f_name, PROGRAM_EXEC_ADDR);
    uint8_t* z = (uint8_t*) PROGRAM_EXEC_ADDR;
    printf("Data Copied\n");
    for(i = 0; i < 32; i++) {
        printf("0x%x, ", *(z + i));
    }

    // Create a process control block for our program in the kernel stack
    pcb_t * proc_ctrl_blk = (pcb_t*) (_8MB - (_8KB)*(curr_proc_id + 1));

    // Grab and store the ESP and EBP in the PCB
    uint32_t esp;
    uint32_t ebp;

    asm volatile("movl %%esp, %0":"=g"(esp));
    asm volatile("movl %%ebp, %0":"=g"(ebp));

    printf("ESP: %d, EBP: %d\n", esp, ebp);

    proc_ctrl_blk->p_ksp = esp;
    proc_ctrl_blk->p_ksp = ebp;

    // Store Proc ID
    proc_ctrl_blk->proc_num = curr_proc_id;

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

    jmp_usr_exec(entrypoint + PROGRAM_EXEC_ADDR);

    return 0;
}

int32_t read (int32_t fd, void * buf, int32_t nbytes) {
    return -1;
}

int32_t write (int32_t fd, const void * buf, int32_t nbytes) {
    return -1;
}

int32_t open (const uint8_t * filename) {
    if(strncmp("stdin", filename, 5) == 0){
        files_array[0].operations_pointer = stdin_ops_table;
        files_array[0].inode = NULL;
        files_array[0].flags = IN_USE;
    }

    if(strncmp("stdout", filename, 6) == 0){
        files_array[1].operations_pointer = stdout_ops_table;
        files_array[1].inode = NULL;
        files_array[1].flags = IN_USE;
    }

    dentry_t file_info;
    int32_t check = read_dentry_by_name(filename, &file_info);

    //checking if the file name exists
    if(check == -1 || files_in_use > MAX_FILES) {
        return -1;
    }

    int i = 0;
    for(i = 2; i < MAX_FILES; i++) {
        if(files_array[i].flags == NOT_USE) {
            switch(file_info.file_type) {
                case 0:
                    files_array[i].operations_pointer = rtc_ops_table;
                    files_array[i].inode = NULL;
                    files_array[i].file_position = 0;
                    break;
                case 1:
                    files_array[i].operations_pointer = dir_ops_table;
                    files_array[i].inode = NULL;
                    files_array[i].file_position = 0;
                    break;
                case 2:
                    files_array[i].operations_pointer = files_ops_table;
                    files_array[i].inode = file_info.inode_num;
                    files_array[i].file_position = 0;
                    break;
            }

            files_array[i].flags = IN_USE;
            files_in_use++;
            return i;
        }
    }


    return -1;
}

int32_t close (int32_t fd) {
    if(fd >= 2 && fd <= 7)
    {
        files_array[fd].flags = NOT_USE;
        files_in_use--;
        return 0;
    }
    return -1;
}

int32_t getargs (uint8_t * buf, int32_t nbytes) {
    return -1;
}

int32_t vidmap (uint8_t ** screen_start) {
    return -1;
}

int32_t set_handler (int32_t signum, void * handler_address) {
    return -1;
}

int32_t sigreturn (void) {
    return -1;
}

void copy_args(const uint8_t* input, uint32_t nbytes) {
        if(input[0] == ' ') {
            args = NULL;
            return;
        }

        uint32_t i = 0;
        uint32_t arg_length = 0;

        for(i = index + 1; i < nbytes; i++) {
            args[i - index - 1] = input[i];
            arg_length++;
        }
        args[arg_length] = '\0';
}

/**
 * Returns the first word of the input string
 * @param  input String to find first word of
 * @return       Pointer to the first word of the string
 */
uint8_t* simple_strtok(const uint8_t* input) {
    uint32_t len = strlen((int8_t*) input);
    uint8_t* output = (uint8_t*)"PLACEHOLDERPLACEHOLDERPLACEHOLDER";
    //No Length string
    if(len == 0) {
        output = NULL;
        return output;
    } else if(input == NULL) {  // NULL input
        output = NULL;
        return output;
    } else if(input[0] == ' ') { // Blank input start
        output = NULL;
        return output;
    } else {
        uint32_t i = 0;
        for(i = 0; i < len+1; i++) {
            if(input[i] == ' ' || input[i] == '\n' || input[i] == '\0') {
                break;
            }
            output[i] = input[i];
        }
        output[i] = '\0';
        index = i;
    }
    return output;
}
