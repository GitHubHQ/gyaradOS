#include "mouse.h"

uint8_t mouse_data[3];

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

int32_t mouse_write(int32_t fd, const uint8_t* buf, int32_t nbytes) {
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

void mouse_init() {
	mouse_ack(1);
	outb(0xA8, 0x64);

	mouse_ack(1);
	outb(0x20, 0x64);

	mouse_ack(0);
	uint8_t currStatus = (inb(0x60) | 2);
	
	mouse_ack(1);
	outb(0x60, 0x64);

	mouse_ack(1);
	outb(currStatus, 0x60);

	uint8_t writeVal = 0xF6;
	mouse_write(NULL, &writeVal, 1);
	mouse_read(NULL, NULL, NULL);

	writeVal = 0xF4;
	mouse_write(NULL, &writeVal, 1);
	mouse_read(NULL, NULL, NULL);
}

void mouse_handle_interrupt() {
	mouse_data[0] = mouse_read(NULL, NULL, NULL);
	mouse_data[1] = mouse_read(NULL, NULL, NULL);
	mouse_data[2] = mouse_read(NULL, NULL, NULL);
	int i;
	for(i = 0; i < 3; i++) {
		printf("%x\n", mouse_data[i]);
	}
}
