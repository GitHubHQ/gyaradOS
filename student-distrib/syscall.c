#include "syscall.h"

files_array files_array[MAX_FILES];

uint32_t stdin_ops_table[4] = {NULL, (uint32_t *) terminal_read, NULL, NULL};
uint32_t stdout_ops_table[4] = {NULL, NULL, (uint32_t *) terminal_write, NULL};
uint32_t rtc_ops_table[4] = {rtc_open, rtc_read, rtc_write, rtc_close};
uint32_t dir_ops_table[4] = {dir_open, dir_read, dir_write, dir_close};
uint32_t files_ops_table[4] = {fs_open, fs_read, fs_write, fs_close};


uint32_t files_in_use = 2;

int32_t halt (uint8_t status) {
    return -1;
}

int32_t execute (const uint8_t * command) {
    // declare the data associated with the filename
    uint8_t file_name[MAX_FILENAME_LENGTH];

    // declare magic numbers 
    uint8_t buf[NUM_MAGIC_NUMS];
    uint8_t magic_nums[NUM_MAGIC_NUMS] = {MAGIC_NUM_1, MAGIC_NUM_2, MAGIC_NUM_3, MAGIC_NUM_4};

    // error if command is null
    if(command == NULL || command == NULL_CHAR) {
        return -1;
    }

    // copy over filename including null termination
    strcpy((int8_t *) file_name, (int8_t *) command);

    // read the first four bytes of the file
    if(-1 == fs_read((int8_t*) file_name, buf, NUM_MAGIC_NUMS)) {
        return -1;
    }

    // check to see if its executable
    if(0 != strncmp((int8_t *) buf, (int8_t *) magic_nums, NUM_MAGIC_NUMS)) {
        return -1;
    }

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
