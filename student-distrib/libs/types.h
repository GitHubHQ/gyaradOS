/* types.h - Defines to use the familiar explicitly-sized types in this
 * OS (uint32_t, int8_t, etc.).  This is necessary because we don't want
 * to include <stdint.h> when building this OS
 * vim:ts=4 noexpandtab
 */

#ifndef _TYPES_H
#define _TYPES_H

#define NULL 0

#ifndef ASM

/* Types defined here just like in <stdint.h> */
typedef int int32_t;
typedef unsigned int uint32_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef char int8_t;
typedef unsigned char uint8_t;

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

typedef int32_t (*func_ptr)();

typedef struct {
    func_ptr * operations_pointer;
    inode_t * inode;
    uint32_t file_position;
    uint32_t flags;
    uint8_t file_name[32];
} file_array;

typedef struct {
    file_array fds[8];
    uint8_t args[64];
    uint8_t proc_num;
    uint32_t base;
    uint32_t ksp;
    uint32_t kbp;
    uint32_t p_ksp;
    uint32_t p_kbp;
    struct pcb_t * prev;
} pcb_t;

#endif /* ASM */

#endif /* _TYPES_H */
