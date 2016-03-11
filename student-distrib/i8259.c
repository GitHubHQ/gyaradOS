/* i8259.c - Functions to interact with the 8259 interrupt controller
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7 */
uint8_t slave_mask; /* IRQs 8-15 */

unsigned int irq_mask_cache = 0xffff;

/**
 * Initialize the 8259 PIC
 */
void i8259_init(void) {

	// Define Master and Slave ports
	outb(ICW1, ICW2_MASTER);
	outb(ICW1, SLAVE_8259_PORT);

	// Set Master offset in the IDT
	outb(ICW2_MASTER, MASTER_8259_PORT_2);
	outb(ICW2_SLAVE, SLAVE_8259_PORT_2);

	// Set Slave offset
	outb(ICW3_MASTER, MASTER_8259_PORT_2);
	outb(ICW3_SLAVE, SLAVE_8259_PORT_2);

	// Define as master and slave
	outb(ICW4_MASTER, MASTER_8259_PORT_2);
	outb(ICW4_SLAVE, SLAVE_8259_PORT_2);
}

/**
 * Enable (unmask) the specified IRQ
 * @param irq_num IRQ to unmask
 */
void enable_irq(uint32_t irq_num) {
	// Find which bit to unmask
	unsigned int mask = ~(1 << irq_num);
	unsigned long flags;

	// Disable interrupts
	cli_and_save(flags);
	irq_mask_cache |= mask;
	// Save mask and write to port
	if(irq_num & 8) {
		outb(SLAVE_8259_PORT_2, irq_mask_cache);
	} else {
		outb(MASTER_8259_PORT_2, irq_mask_cache);
	}
	restore_flags(flags);
	sti();
}

/**
 * Disable (mask) the specified IRQ 
 * @param irq_num IRQ to mask
 */
void disable_irq(uint32_t irq_num){
	// Find which bit to mask
	unsigned int mask = 1 << irq_num;
	unsigned long flags;

	// Disable interrupts
	cli_and_save(flags);
	irq_mask_cache |= mask;
	// Save mask and write to port
	if(irq_num & 8) {
		outb(SLAVE_8259_PORT_2, irq_mask_cache);
	} else {
		outb(MASTER_8259_PORT_2, irq_mask_cache);
	}
	restore_flags(flags);
	sti();
}

/**
 * Send end-of-interrupt signal for the specified IRQ
 * @param irq_num IRQ to send EOI to
 */
void send_eoi(uint32_t irq_num){
	if (irq_num & 8)
	{
		outb(SLAVE_8259_PORT, END_OF_INTERRUPT);
	} else {
		outb(MASTER_8259_PORT, END_OF_INTERRUPT);
	}
}

