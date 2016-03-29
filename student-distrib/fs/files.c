#include "files.h"

boot_block b;
uint32_t b_block_addrs;
uint32_t read_location;

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

    read_location = 0;
}

int32_t fs_read(int8_t* fd, uint8_t * buf, int32_t nbytes) {
	dentry_t temp;
	read_dentry_by_name(fd, &temp);
    int bytesRead = read_data(temp.inode_num, read_location, buf, nbytes);
        read_location += bytesRead;
    return bytesRead;
}

int32_t fs_write(int8_t* fd, uint8_t * buf, int32_t nbytes) {
    return -1;
}

int32_t fs_open(){
    return 0;
}

int32_t fs_close(){
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

    int readBytes = 32; //32 is the max bytes for a file name
    if (strlen(fname) >= 32) 
    {
        readBytes = 31;
    }

    //loop through dir entries until file name is found
    for(i = 0; i < b.n_dentries; i++) {
    	//printf("%s\n", dentries[i].file_name);
        if(0 == strncmp((int8_t *) dentries[i].file_name, (int8_t *) fname, readBytes)) {
            strncpy((int8_t *) dentry->file_name, (int8_t *) dentries[i].file_name, 32);
            dentry->file_type = dentries[i].file_type;
            dentry->inode_num = dentries[i].inode_num;

            return 0;
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

void test_fs() {
		//reading a non txt file
    char * fname = "cat";
    printf("Reading cat... \n");
    dentry_t temp;
	read_dentry_by_name(fname, &temp);
    printf("Size of flie: %d\n", inodes[temp.inode_num].file_size);

    uint8_t buf[10];
    fs_read(fname, buf, 10);
    int i = 0;
    while(0 !=  fs_read(fname, buf, 10))
    {
        for(i = 0 ; i < 10; i++){
            printf("%x", buf[i]);
        }
        //printf("\n");
    }
    printf("Done\n");

	//reading a txt file
	// char * fname = "frame1.txt";
	// printf("Reading frame1.txt... \n");
 //        dentry_t temp;
	// read_dentry_by_name(fname, &temp);
 //    printf("Size of flie: %d\n", inodes[temp.inode_num].file_size);
 //    int i;
 //    uint8_t buf[10];
 //    while(0 !=  fs_read(fname, buf, 10))
 //    {
 //        for(i = 0 ; i < 10; i++){
 //            printf("%c", buf[i]);
 //        }
 //    	//printf("\n");
 //    }
 //    printf("Done\n");
 //    fs_read(fname, buf, 10);
 //    for(i = 0 ; i < 10; i++)
 //    printf("%d, ", buf[i]);

	// reading a large file
	// char * fname = "verylargetxtwithverylongname.txt";
	// printf("Reading verylargetxtwithverylongname.txt... \n");
 //    dentry_t temp;
	// read_dentry_by_name(fname, &temp);
 //    printf("Size of flie: %d\n", inodes[temp.inode_num].file_size);
 //    int i;
 //    uint8_t buf[10];
 //    while(0 !=  fs_read(fname, buf, 10))
 //    {
 //        for(i = 0 ; i < 10; i++){
 //            printf("%c", buf[i]);
 //        }
 //        //printf("\n");
 //    }
 //    printf("Done\n");
 }

int32_t dir_open(){
    return 0;
}

int32_t dir_close(){
    return 0;
}


int32_t dir_write(){
    return -1;
}

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

void test_dir_read(){
    
    int32_t fd = 0, cnt = 0;
    uint8_t buf[33];

    printf("Testing dir_read...\n");
    
    //print out all files in directory
    while(0 != (cnt = dir_read(fd,buf,32))){
        printf("%s\n",buf);
    }
}
