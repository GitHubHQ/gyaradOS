/*
 * keyboard.h - Defines used in interactions with the keyboard
 */

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

// Data I/O port for the keyboard
#define KEYBOARD_D_PORT 0x60

// Reads a scan code from the keyboard port
unsigned char kbrd_read_scan_code(void);

#endif  /* _KEYBOARD_H */
