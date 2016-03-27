#ifndef _FILES_H
#define _FILES_H

#include "../libs/types.h"
#include "../libs/lib.h"

#define BOOT_BLOCK_SIZE 64
#define DENTRY_SIZE 64

typedef struct {
	uint32_t file_size;
	uint32_t blocks[1023];
} i_node;

typedef struct {
	uint8_t file_name[32];
	uint8_t file_type;
	uint32_t inode_num;
	uint32_t blocks_reserved[24];
} dentry_t;

typedef struct {
	uint32_t dir_entries;
	uint32_t inodes;
	uint32_t data_blocks;
	uint32_t blocks_reserved[52];
	dentry_t* dentry_entries;
} boot_block;

int32_t read_dentry_by_name (const uint8_t* fname, dentry_t * dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t * dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length);

void fs_init(uint32_t addrs);
int32_t fs_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t fs_read(int32_t fd, void* buf, int32_t nbytes);
int32_t fs_open();
int32_t fs_close();

#endif
