/* rtc.c - Functions to interact with the real time clock
 */
#include "rtc.h"

void rtc_init() {
	// Disable interrupts
	cli();
	// Select register B
	outb(RTC_NMI_INIT_VAL, RTC_REG_NUM_PORT);
	// Read current value of register B
	char previous = inb(RTC_RW_CMOS_PORT);
	// Set index 
	outb(RTC_NMI_INIT_VAL, RTC_REG_NUM_PORT);
	// Enable bit 6 of register B, enabling interrupts from RTC
	outb(previous | RTC_REG_B_V_MASK, RTC_RW_CMOS_PORT);
	// Enable interrupts
	sti();
}

void rtc_special_eoi() {
	// Select register C
	outb(RTC_REG_C_SELECT, RTC_REG_NUM_PORT);
	// Throw away data for next interrupt
	inb(RTC_RW_CMOS_PORT);
}

int rtc_set_frequency(int rate) {
	if (rate > 15 || rate < 0) {
		return -1;
	}

	rate &= 0x0F;
	cli();
	outb(RTC_REG_A_SELECT, RTC_REG_NUM_PORT);
	char previous = inb(RTC_RW_CMOS_PORT);
	outb(RTC_REG_A_SELECT, RTC_REG_NUM_PORT);
	outb((previous & RTC_REG_A_V_MASK) | rate, RTC_RW_CMOS_PORT);
	sti();
	return 0;
}
