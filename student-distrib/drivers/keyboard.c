/* keyboard.c - Functions to interact with the keyboard
 */

#include "lib.h"

/**
 * Reads a scan code from the keyboard port
 * @return  The scan code sent from the keyboard (Not ASCII)
 */
unsigned char kbrd_read_scan_code(void) {
	return inb(KEYBOARD_D_PORT);
}
