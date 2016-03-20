/* rtc.c - Functions to interact with the real time clock
 */
#include "rtc.h"

int32_t init_mode;

volatile int32_t intr_occ = 0;

/**
 * Sets the frequency divisor for the RTC clock
 * @param frequency A value between 2 and 1024 inclusive to set the rate to
 */
int32_t rtc_set_frequency(int32_t frequency) {
	float rateVal = logBase2(frequency);
	int32_t rate;

	// Error checking
	if (frequency < 2 || frequency > 1024)
	{
		return -1;
	}

	// Be nice to the user, round up for down for them if they don't give a power of 2
	if( ((int)((int)(rateVal * 10) % 10)) > 5) {
		rate = ceil(rateVal);
	} else {
		rate = floor(rateVal);
	}

	// Get the actual divider
	rate = 16 - rate;

	// Normalize the rate
	rate &= 0x0F;
	// Disable interrupts
	cli();
	// Select the A register
	outb(RTC_REG_A_SELECT, RTC_REG_NUM_PORT);
	// Grab the current value
	char previous = inb(RTC_RW_CMOS_PORT);
	// Set index
	outb(RTC_REG_A_SELECT, RTC_REG_NUM_PORT);
	// Write the new divisor value for rate
	outb((previous & RTC_REG_A_V_MASK) | rate, RTC_RW_CMOS_PORT);
	// Enable interrupts
	sti();
	// Success
	return 0;
}

/**
 * Initalizes the RTC to default interrupt values (1024 Hz)
 */
void rtc_init(int mode) {
	// Disable interrupts
	cli();
	init_mode = mode;
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

	rtc_set_frequency(RTC_DEFAULT_HZ);
}

/**
 * A seperate EOI that lets the RTC clock generate more interrupts
 * This EOI is different from the one that resets the 8259 PIC.
 * This EOI must be called after the 8259 EOI is called if more
 * 	RTC interrupts are to be generated.
 */
void rtc_special_eoi() {
	// Select register C
	outb(RTC_REG_C_SELECT, RTC_REG_NUM_PORT);
	// Throw away data for next interrupt
	inb(RTC_RW_CMOS_PORT);
}

/**
 * Handle a rtc interrupt based on how the RTC was initalized
 */
void rtc_handle_interrupt(void) {
	cli();
	intr_occ = 1;
	// Print based on mode
	if(init_mode == RTC_SILENT) {
		/* Do nothing */
	} else if (init_mode == RTC_VERBOSE) {
		printf("RTC Interrupt!\n");
	} else {
		test_interrupts();
	}
	// Send PIC EOI
	send_eoi(IRQ_RTC);
	// Send RTC EOI
    rtc_special_eoi();
	sti();
}

int32_t rtc_open(void) {
	rtc_set_frequency(RTC_DEFAULT_HZ);
	return 0;
}

int32_t rtc_close(void) {
	//Reset frequency and return
	rtc_set_frequency(RTC_DEFAULT_HZ);
	return 0;
}

int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes) {
	while(!intr_occ) {
		/* Do nothing */
	}

	intr_occ = 0;

	return 0;
}

int32_t rtc_write (int32_t fd, int32_t* buf, int32_t nbytes) {
	if (buf == NULL)
	{
		return -1;
	}
	int32_t frequency = *buf;
	return rtc_set_frequency(frequency);
}
