#include "files.h"

boot_block b;
uint32_t b_block_addrs;

dentry_t * dentries;
inode_t * inodes;
uint32_t data_blocks;
uint32_t dirReads;

/**
 * Initializing the Boot block by getting the data from address
 * @param addrs The address of the boot module
 */
void fs_init(uint32_t addrs) {
    b_block_addrs = addrs;
    memcpy(&b, (void *) b_block_addrs, BOOT_BLOCK_SIZE);

    dentries = (dentry_t *)(b_block_addrs + BOOT_BLOCK_SIZE);
    inodes = (inode_t *) (b_block_addrs + BLOCK_SIZE);
    data_blocks = b_block_addrs + b.n_inodes * BLOCK_SIZE + BLOCK_SIZE;

    dirReads = 0;
}

/* fs_read(int8_t* fd, uint8_t * buf, int32_t nbytes)
 * inputs: file descriptor fd, output buffer buf, and number of bytes to read
 * nbytes
 * ouputs: return number of bytes read into the buffer for output
 */
int32_t fs_read(int32_t fd, uint8_t * buf, int32_t nbytes) {
    dentry_t temp;

    if(0 != read_dentry_by_name((uint8_t*)fd, &temp)) {
        return -1;
    }

    int bytesRead = read_data(temp.inode_num, 0, buf, nbytes);

    return bytesRead;
}

/* fs_write()
 * inputs: none
 * ouputs: none return -1 because read only
 */
int32_t fs_write(int32_t fd, const uint8_t * buf, int32_t nbytes) {
    return -1;
}

/* fs_open()
 * inputs: none
 * ouputs: none return 0
 */
int32_t fs_open(const uint8_t* filename){
    
    return 0; 
}

/* fs_close()
 * inputs: none
 * ouputs: none return 0
 */
int32_t fs_close(void){
    return 0;
}

/** 
 * Copies the data of given name to given dentry
 * @param  fname  File name that user wants to copy
 * @param  dentry Where to copy
 * @return        0 if success, -1 if failure
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry) {
    int i;

    int readBytes = strlen((int8_t *) fname);

    if (readBytes > MAX_FILENAME_LENGTH) {
        readBytes = MAX_FILENAME_LENGTH;
    }

    //loop through dir entries until file name is found
    for(i = 0; i < b.n_dentries; i++) {
        if(strlen((int8_t *) fname) == strlen((int8_t *) dentries[i].file_name)) {
            if(0 == strncmp((int8_t *) dentries[i].file_name, (int8_t *) fname, readBytes)) {
                strncpy((int8_t *) dentry->file_name, (int8_t *) dentries[i].file_name, readBytes);
                dentry->file_type = dentries[i].file_type;
                dentry->inode_num = dentries[i].inode_num;
                return 0;
            }
        }
    }

    //reached end of entries
    return -1;
}

/**
 * Finds and copies the data at given index(if it exists) into given dentry.
 * @param  index  The index of the dentry that user wants to copy
 * @param  dentry Where to copy the information
 * @return        0 if success, -1 if failure
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry) {
    // //check if index out of bounds
    if(index >= b.n_dentries || index < 0)
        return -1;

    //copying the data from the index to the dentry
    strncpy((int8_t *)dentry->file_name, (int8_t *)dentries[index].file_name, 32);
    dentry->file_type = dentries[index].file_type;
    dentry->inode_num = dentries[index].inode_num;

    return 0;
}

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length) {
    if(inode >= b.n_inodes || inode < 0)
        return -1;

    if(offset >= inodes[inode].file_size)
        return 0;

    int curr_block = offset / BLOCK_SIZE;
    int location_in_block = offset % BLOCK_SIZE;

    uint8_t * curr_read_pos = (uint8_t *) (data_blocks + inodes[inode].blocks[curr_block] * BLOCK_SIZE + location_in_block);

    int num_reads = 0;
    int buf_pos = 0;
     
    while(num_reads < length) {
        buf[buf_pos] = *curr_read_pos;
        num_reads++;
        buf_pos++;
        curr_read_pos++;
        location_in_block++;

        if(offset + num_reads > inodes[inode].file_size) {
            return num_reads;
        }

        if(location_in_block >= BLOCK_SIZE) {
            // reset location
            location_in_block = 0;
            curr_block++;
            
            //checking if the data block is invalid
            if(inodes[inode].blocks[curr_block] >= b.n_data_blocks) {
                return -1;
            }

            // recalculate read position
            curr_read_pos = (uint8_t *) (data_blocks + inodes[inode].blocks[curr_block] * BLOCK_SIZE + location_in_block);
        }
    }

    return num_reads;
}

int32_t copy_file_to_addr(uint8_t* fname, uint32_t addr) {
    dentry_t temp_dentry;

    if(0 != read_dentry_by_name(fname, &temp_dentry)) {
        return -1;
    }

    uint32_t file_size = inodes[temp_dentry.inode_num].file_size;
    uint8_t buf[file_size];
    
    uint32_t amount_to_copy = read_data(temp_dentry.inode_num, 0, buf, file_size);

    if(amount_to_copy == -1) {
        return -1;
    }

    memcpy((uint32_t *) addr, buf, amount_to_copy);

    return 0;
}

inode_t* get_inode(uint32_t inode_num) {
    return inodes + inode_num * BLOCK_SIZE;
}
/* test_fs()
 * description: tests the filesystem functions
 * input: none
 * output:none
 */
void test_fs() {
    //reading a non txt file
    // char * fname = "cat";
    // printf("Reading cat... \n");
    // dentry_t temp;
    // read_dentry_by_name((uint8_t*)fname, &temp);
    // printf("Size of flie: %d\n", inodes[temp.inode_num].file_size);

    // uint8_t buf[10];
    // fs_read(fname, buf, 10);
    // int i = 0;
    // while(0 != fs_read(fname, buf, 10))
    // {
    //     for(i = 0 ; i < 10; i++){
    //         printf("%x", buf[i]);
    //     }
    //     //printf("\n");
    // }

    //reading a txt file
    // char * fname = "frame1.txt";
    // printf("Reading frame1.txt... \n");
    //     dentry_t temp;
    // read_dentry_by_name((uint8_t*)fname, &temp);
    // printf("Size of flie: %d\n", inodes[temp.inode_num].file_size);
    // int i;
    // uint8_t buf[10];
    // while(0 !=  fs_read(fname, buf, 10))
    // {
    //     for(i = 0 ; i < 10; i++){
    //         printf("%c", buf[i]);
    //     }
    //     //printf("\n");
    // }

    // reading a large file
    // char * fname = "verylargetxtwithverylongname.txt";
    // printf("Reading verylargetxtwithverylongname.txt... \n");
    // dentry_t temp;
    // read_dentry_by_name((uint8_t*)fname, &temp);
    // printf("Size of flie: %d\n", inodes[temp.inode_num].file_size);
    // int i;
    // uint8_t buf[10];
    // while(0 !=  fs_read(fname, buf, 10))
    // {
    //     for(i = 0 ; i < 10; i++){
    //         printf("%c", buf[i]);
    //     }
    //     //printf("\n");
    // }
 }

/* dir_open()
 * inputs: none
 * ouputs: none return 0
 */
int32_t dir_open(const uint8_t* filename){
    return 0;
}

/* dir_close()
 * inputs: none
 * ouputs: none return 0
 */
int32_t dir_close(void){
    return 0;
}

/* dir_write()
 * inputs: none
 * outputs: return -1 because read only
 */
int32_t dir_write(int32_t fd, const int8_t * buf, int32_t nbytes){
    return -1;
}

/* dir_read(int32_t fd, int8_t * buf, int32_t length)
 * description: reads next filename into the buffer eveytime dir_read is called
 * inputs: file descriptor, buffer for output, and length for bytes to copy
 * outputs: copies filename into buf and returns the number of bytes copied
 */
int32_t dir_read(int32_t fd, int8_t * buf, int32_t length){
    //check if reached end       
    if(dirReads >= b.n_dentries){
        dirReads = 0;
        return 0;
    }
    //copy name into buf
    strcpy(buf, dentries[dirReads].file_name);
    
    //get number of bytes and increment the directory read counter
    int bytesCopied = strlen(buf);
    dirReads++;

    return bytesCopied;
}

/* test_dir_read()
 * description: this function tests printing out all files in the directory
 * inputs: none
 * outputs: none
 */
void test_dir_read() {
    int32_t fd = 0, cnt = 0;
    int8_t buf[33];

    printf("Testing dir_read...\n");
    
    //print out all files in directory
    while(0 != (cnt = dir_read(fd,buf,32))){
        printf("%s\n",buf);
    }
}
