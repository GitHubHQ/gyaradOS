#include "files.h"

boot_block b;
uint32_t b_block_addrs;
//dentry_t * dentry_entries;

/**
 * Initializing the Boot block by getting the data from address
 * @param addrs The address of the boot module
 */
void fs_init(uint32_t addrs)
{
	b_block_addrs = addrs;
	memcpy(&b, (void *)b_block_addrs, BOOT_BLOCK_SIZE);
	b.dentry_entries = (dentry_t *)b_block_addrs + BOOT_BLOCK_SIZE;
	//dentry_addrs = b_block_addrs + BOOT_BLOCK_SIZE;
}

int32_t fs_read(int32_t fd, void* buf, int32_t nbytes){
    return 0;
}

int32_t fs_write(int32_t fd, const void* buf, int32_t nbytes){
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

	//loop through dir entries until file name is found
	for(i = 0; i < b.dir_entries; i++)
	{
		if(0 == strncmp((int8_t *)b.dentry_entries[i].file_name, (int8_t *)fname, 32)) //32 is the max length of a file name
	 	{
	 		strcpy((int8_t *)dentry->file_name, (int8_t *)b.dentry_entries[i].file_name);
	 		dentry->file_type = b.dentry_entries[i].file_type;
	 		dentry->inode_num = b.dentry_entries[i].inode_num;
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
	//check if index out of bounds
	if(index >= b.dir_entries || index < 0)
	 	return -1;

	//copying the data from the index to the dentry
	strcpy((int8_t *)dentry->file_name, (int8_t *)b.dentry_entries[index].file_name);
	dentry->file_type = b.dentry_entries[index].file_type;
	dentry->inode_num = b.dentry_entries[index].inode_num;

    return 0;
}

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length) {
	if(inode >= b.inodes || inode < 0)
		return -1;
	
    return 0;
}
