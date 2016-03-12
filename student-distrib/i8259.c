/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7 */
uint8_t slave_mask = 0xFF; /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
	/* Master port */
	outb(ICW1, MASTER_8259_PORT);
	/* Slave port */
	outb(ICW1, SLAVE_8259_PORT);

	/* Master port */
	outb(ICW2_MASTER, MASTER_8259_PORT + 1);
	/* Slave port */
	outb(ICW2_SLAVE, SLAVE_8259_PORT + 1);

	/* Master port */
	outb(ICW3_MASTER, MASTER_8259_PORT + 1);
	/* Slave port */
	outb(ICW3_SLAVE, SLAVE_8259_PORT + 1);

	/* Master port */
	outb(ICW4_MASTER, MASTER_8259_PORT + 1);
	/* Slave port */
	outb(ICW4_SLAVE, SLAVE_8259_PORT + 1);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
	if(irq_num < 0 || irq_num > 15) {
		return;
	}

	unsigned int mask = ~(1 << irq_num);
	unsigned long flags;
	
	if((irq_num >= 0 && irq_num < 8)) {
		master_mask &= mask;
		cli_and_save(flags);
		outb(master_mask, MASTER_8259_PORT + 1);
		restore_flags(flags);
		sti();
		return;
	} else if(irq_num > 7 && irq_num < 16) {
		slave_mask &= mask;
		cli_and_save(flags);
		outb(slave_mask, SLAVE_8259_PORT + 1);
		restore_flags(flags);
		sti();
		return;
	}
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
	if(irq_num < 0 || irq_num > 15) {
		return;
	}

	unsigned int mask = (1 << irq_num);
	unsigned long flags;

	if((irq_num >= 0 && irq_num < 8)) {
		master_mask &= mask;
		cli_and_save(flags);
		outb(master_mask, MASTER_8259_PORT + 1);
		restore_flags(flags);
		sti();
		return;
	} else if(irq_num > 7 && irq_num < 16) {
		slave_mask &= mask;
		cli_and_save(flags);
		outb(slave_mask, SLAVE_8259_PORT + 1);
		restore_flags(flags);
		sti();
		return;
	}
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
	if((irq_num >= 0 && irq_num < 8)) {
		outb(EOI | irq_num, MASTER_8259_PORT);
	} else if(irq_num > 7 && irq_num < 16) {
		outb(EOI | (irq_num - 8), SLAVE_8259_PORT);
		outb(EOI + 2, MASTER_8259_PORT);
	}
}
