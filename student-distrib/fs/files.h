#ifndef _FILES_H
#define _FILES_H

#include "../libs/types.h"
#include "../libs/lib.h"

#define BOOT_BLOCK_SIZE 64
#define DENTRY_SIZE 64
#define BLOCK_SIZE 0x1000
#define MAX_FILENAME_LENGTH 31

typedef struct {
	uint32_t file_size;
	uint32_t blocks[1023];
} inode_t;

typedef struct {
	int8_t file_name[32];
	uint32_t file_type;
	uint32_t inode_num;
	uint8_t blocks_reserved[24];
} dentry_t;

typedef struct {
	uint32_t n_dentries;
	uint32_t n_inodes;
	uint32_t n_data_blocks;
	uint8_t blocks_reserved[52];
} boot_block;

/* filesystem read functions */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t * dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t * dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length);

/* function used for syscalls */
int32_t copy_file_to_addr(uint8_t* fname, uint32_t addr);

/* filesystem functions */
void fs_init(uint32_t addrs);
int32_t fs_write(int8_t* fd, uint8_t * buf, int32_t nbytes);
int32_t fs_read(int8_t* fd, uint8_t * buf, int32_t nbytes);
int32_t fs_open();
int32_t fs_close();
void test_fs();

/* directory functions */
void test_dir_read();
int32_t dir_read(int32_t fd, int8_t * buf, int32_t length);
int32_t dir_write();
int32_t dir_open();
int32_t dir_close();


#endif
