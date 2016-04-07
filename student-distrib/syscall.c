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

    pcb_t* prev_proc = curr_proc;

    // declare the data associated with the filename
    uint8_t file_name[MAX_FILENAME_LENGTH];

    // Holder for program name
    uint8_t * cmd_name = simple_strtok(command);


    // declare magic numbers 
    uint8_t buf[NUM_BYTES_STATS];
    uint8_t magic_nums[NUM_MAGIC_NUMS] = {MAGIC_NUM_1, MAGIC_NUM_2, MAGIC_NUM_3, MAGIC_NUM_4};

    uint32_t entry_point_addr = 0;
    uint32_t curr_read_entry_point = ENTRY_POINT_START;

    // Counter variable
    uint32_t i;
    // mask used to search for next available procId
    uint32_t proc_search_mask = PROGRAM_LOCATION_MASK;

    // error if command string is null
    if(command == NULL || command == NULL_CHAR) { 
        return -1;
    }

    // copy over filename including null termination
    strcpy((int8_t *) file_name, (int8_t *) cmd_name);

    // read the first four bytes of the file
    if(-1 == fs_read((int8_t*) file_name, buf, NUM_BYTES_STATS)) {
        return -1;
    }

    // check to see if its executable
    if(0 != strncmp((int8_t *) buf, (int8_t *) magic_nums, NUM_MAGIC_NUMS)) {
        return -1;
    }

    printf("%s\n", "ERROR CHECK COMPLETE");

    // Get a free process number
    for(i = 0; i < MAX_PROG_NUM; i++) {
        printf("LOOP NUMBER: %d\n", i);
        if(!(proc_search_mask & curr_proc_id_mask)) {
            curr_proc_id_mask |= proc_search_mask;
            curr_proc_id = i;
            break;
        } else {
            proc_search_mask = proc_search_mask >> 1;
        }
    }

    curr_proc = (pcb_t*)(_8MB - (_8KB)*(i+1));

    printf("%s\n", "PROC NUM PRE");

    if(i == MAX_PROG_NUM) {
        return -1;
    }

    printf("%s %d\n", "PROCESS NUMBER FOUND: ", curr_proc->proc_num);

    // get entry point into program
    entry_point_addr |= (buf[curr_read_entry_point]);
    curr_read_entry_point++;
    entry_point_addr |= (buf[curr_read_entry_point] << (1 * 8));
    curr_read_entry_point++;
    entry_point_addr |= (buf[curr_read_entry_point] << (2 * 8));
    curr_read_entry_point++;
    entry_point_addr |= (buf[curr_read_entry_point] << (3 * 8));
    curr_read_entry_point++;

    printf("%s\n", "ENTRY POINT SET");

    // set up the new page directory
    init_new_process(curr_proc->proc_num);
    printf("%s\n", "PAGING ENABLED");
    
    // Set up PCB
    curr_proc->proc_num = i;
    // Should only happen when spawning a initial shell
    if(prev_proc == NULL){
        curr_proc->parent = NULL;
    }

    // Otherwise actually do the setup for the PCB when spawning a proc from the shell
    curr_proc->parent = prev_proc;

    // Initalize file array to empty and null, then set
    for(i = 0; i < 8; i++) {
        curr_proc->fds[i].operations_pointer = NULL;
        curr_proc->fds[i].inode = NULL;
        curr_proc->fds[i].file_position = 0;
        curr_proc->fds[i].flags = 0;
    }

    // Save current pointers here too
    asm volatile("movl %%esp, %0" : "=r" (curr_proc->ksp) );
    asm volatile("movl %%ebp, %0" : "=r" (curr_proc->kbp) );

    // Enable STDIN and STDOUT
    curr_proc->fds[0].operations_pointer = stdin_ops_table[READ];
    curr_proc->fds[0].flags= IN_USE;

    curr_proc->fds[1].operations_pointer = stdout_ops_table[WRITE];
    curr_proc->fds[1].flags = IN_USE;

    printf("%s\n", "PCB SETUP");

    // load the program into the correct starting address
    copy_file_to_addr(cmd_name, PROGRAM_EXEC_ADDR);

    printf("%s\n", "PROGRAM COPIED TO MEMORY");

    tss.esp0 = _8MB - _8KB * curr_proc_id - 4;
    tss.ss0 = KERNEL_DS;

    printf("%s\n", "TSS SETUP");
    
    // jump to the file to execute it
    jmp_usr_exec(entry_point_addr);

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
