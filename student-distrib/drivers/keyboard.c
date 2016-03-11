
#include "lib.h"

#define KEYBOARD_D_PORT 0x60

/**
 * Reads a scan code from the keyboard port
 * @return  The scan code sent from the keyboard
 */
unsigned char kbrd_read_scan_code(void) {
	return inb(KEYBOARD_D_PORT);
}