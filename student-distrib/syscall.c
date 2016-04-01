#include "syscall.h"

int32_t halt (uint8_t status) {
    return -1;
}

int32_t execute (const uint8_t * command) {
    // declare the data associated with the filename
    uint8_t file_name[MAX_FILENAME_LENGTH];

    // Delcare data array
    // data[0] contain the command to execute
    // data[n] contains arguments to the command
    //      arguments are listed until NULL is reached
    //      with a max of 31 arguments (1 command, 31 args)
    uint8_t data[MAX_NUMBER_ARGS][MAX_FILENAME_LENGTH];

    // Temp variable for filling up the data array
    uint8_t *tmp; 

    // declare magic numbers 
    uint8_t buf[NUM_MAGIC_NUMS];
    uint8_t magic_nums[NUM_MAGIC_NUMS] = {MAGIC_NUM_1, MAGIC_NUM_2, MAGIC_NUM_3, MAGIC_NUM_4};

    // Initalize the data array to null
    int i;
    for(i = 0; i < MAX_NUMBER_ARGS; i++) {
        strcpy((int8_t*) data[i], '\0');
    }

    // error if command string is null
    if(command == NULL || command == NULL_CHAR) {
        return -1;
    }

    // Default delimit by space
    tmp = strtok((int8_t*) command, " ");
    // Fill up the data array, leaving blank commands as NULL
    for(i = 0; tmp != NULL; i++) {
        strcpy((int8_t*) data[i], tmp);
        tmp = strtok((int8_t*) command, " ");
    }

    for(i = 0; i < MAX_NUMBER_ARGS; i++) {
        if (data[i] != NULL) {
            printf("%s, ", *data[i]);
        } else {
            printf("%s", "NULL");
        }
    }

    // copy over filename including null termination
    strcpy((int8_t *) file_name, (int8_t *) data[0]);

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
