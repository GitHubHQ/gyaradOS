
#ifndef IRQ_H
#define IRQ_H

// handler for keyboard interrupts
extern void key_irq();

// handler for RTC interrupts
extern void rtc_irq();

// handler for general interrupts
extern void general_irq();

#endif /* IRQ_H*/
