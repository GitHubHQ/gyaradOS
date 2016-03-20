#ifdef _FILES_H
#define _FILES_H

#include "types.h"
#include "lib.h"

struct boot_block {
	uint32_t dir_entries;
	uint32_t inodes;
	uint32_t data_blocks;
	uint32_t blocks_reserved[52];
	dentry_t d_entry[63];
};

struct i_node {
	uint32_t file_size;
	uint32_t blocks[1023];
};

struct dentry_t {
	uint8_t file_name[32];
	uint8_t file_type;
	uint32_t inode_num;
	uint32_t blocks_reserved[24];
};

int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

#endif