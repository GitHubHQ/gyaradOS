#include "files.h"

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

int32_t read_dentry_by_name (const uint8_t* fname, struct dentry_t * dentry) {
    return 0;
}

int32_t read_dentry_by_index (uint32_t index, struct dentry_t * dentry) {
    return 0;
}

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t * buf, uint32_t length) {
    return 0;
}
