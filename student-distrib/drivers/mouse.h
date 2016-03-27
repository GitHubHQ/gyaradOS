/* mouse.h - Defines for the mouse driver
*/
#ifndef _MOUSE_H
#define _MOUSE_H

#include "../libs/types.h"
#include "../libs/lib.h"

#define MOUSE_ACK_WAIT 10000

void mouse_init();
int32_t mouse_read(int32_t fd, int32_t* buf, int32_t nbytes);
int32_t mouse_write(int32_t fd, const uint8_t* buf, int32_t nbytes);
int32_t mouse_ack(uint32_t ack_type);
void mouse_handle_interrupt();

#endif /* _MOUSE_H */
