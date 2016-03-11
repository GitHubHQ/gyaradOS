#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "lib.h"

#define KEYBOARD_D_PORT     0x60

extern unsigned char kbrd_read_scan_code();

#endif  /* _KEYBOARD_H */
