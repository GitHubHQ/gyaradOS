#include "syscall.h"

int32_t halt (uint8_t status) {
    return -1;
}

int32_t execute (const uint8_t * command) {
    int i = 0;
    int j = 0;

    // declare the data associated with the filename
    uint8_t file_name[MAX_FILENAME_LENGTH];
    char data[32][32];
    int curr_data_index = 0;
    int curr_str_index = 0;

    // declare magic numbers 
    uint8_t buf[NUM_BYTES_STATS];
    uint8_t magic_nums[NUM_MAGIC_NUMS] = {MAGIC_NUM_1, MAGIC_NUM_2, MAGIC_NUM_3, MAGIC_NUM_4};

    uint32_t entry_point_addr = 0;
    uint32_t curr_read_entry_point = ENTRY_POINT_START;

    // error if command string is null
    if(command == NULL || command == NULL_CHAR) {
        return -1;
    }

    for(i = 0; i < 32; i++) {
        for(j = 0; j < 32; j++) {
            data[i][j] = ' ';
        }
    }

    for(i = 0; i < strlen(command); i++) {
        if(command[i] == ' ') {
            data[curr_data_index][curr_str_index] = '\0';
            curr_data_index++;
            curr_str_index = 0;
        } else {
            data[curr_data_index][curr_str_index] = command[i];
            curr_str_index++;
        }
    }

    // copy over filename including null termination
    strcpy((int8_t *) file_name, (int8_t *) data[0]);

    // read the first four bytes of the file
    if(-1 == fs_read((int8_t*) file_name, buf, NUM_BYTES_STATS)) {
        return -1;
    }

    // check to see if its executable
    if(0 != strncmp((int8_t *) buf, (int8_t *) magic_nums, NUM_MAGIC_NUMS)) {
        return -1;
    }

    // get entry point into program
    entry_point_addr |= (buf[curr_read_entry_point] << (3 * 8));
    curr_read_entry_point++;
    entry_point_addr |= (buf[curr_read_entry_point] << (2 * 8));
    curr_read_entry_point++;
    entry_point_addr |= (buf[curr_read_entry_point] << (1 * 8));
    curr_read_entry_point++;
    entry_point_addr |= (buf[curr_read_entry_point]);
    curr_read_entry_point++;

    // set up the new page directory
    
    // load the program into the correct starting address
    copy_file_to_addr(data[0], PROGRAM_EXEC_ADDR);
    
    // assign a pcb with the new PID
    
    // save things
    
    // jump to the file to execute it
    user_exec(entry_point_addr);

    return 0;
}

int32_t read (int32_t fd, void * buf, int32_t nbytes) {
    return -1;
}

int32_t write (int32_t fd, const void * buf, int32_t nbytes) {
    return -1;
}

int32_t open (const uint8_t * filename) {
    return -1;
}

int32_t close (int32_t fd) {
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
