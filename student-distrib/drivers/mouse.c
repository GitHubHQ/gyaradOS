#include "mouse.h"

int32_t mouse_ack(uint32_t ack_type) {
	uint32_t mouse_wait;
	mouse_wait = MOUSE_ACK_WAIT;
	if(ack_type) {
		while(mouse_wait--) {
			if (!(inb(0x64) & 2)) {
				return 0;
			}
		}
		printf("ERROR: Mouse acknowledge timeout!\n");
		return -1;
	} else {
		while(mouse_wait--) {
			if ((inb(0x64) & 1)) {
				return 0;
			}
		}
		printf("ERROR: Mouse acknowledge timeout!\n");
		return -1;
	}
}

int32_t mouse_write(int32_t fd, const int8_t* buf, int32_t nbytes) {
	// Check if params valid
	if(buf == NULL) {
		return -1;
	}
	if(nbytes != 1) {
		return -1;
	}

	// Check if mouse ready, else wait
	if(mouse_ack(1)) {
		return -1;
	}

	// Send mouse command prep
	outb(0xD4, 0x64);

	// Check mouse ack
	if (mouse_ack(1)) {
		return -1;
	}

	// Write to mouse
	outb(*buf, 0x60);

	return 0;
}

int32_t mouse_read(int32_t fd, int32_t* buf, int32_t nbytes) {
	// Wait for mouse ready
	if(mouse_ack(0)) {
		return -1;
	}
	// Return current mouse values
	return inb(0x60);
}

int32_t mouse_init() {
	return 0;
}
