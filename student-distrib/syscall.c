#include "syscall.h"

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
