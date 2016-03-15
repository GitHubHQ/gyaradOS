/* rtc.h - Defines for real time clock functions
 */
#ifndef _RTC_H
#define _RTC_H

#include "../libs/lib.h"
#include "i8259.h"
#include "../libs/math.h"

/* Constants for port selection */
#define RTC_REG_NUM_PORT 0x70
#define RTC_RW_CMOS_PORT 0x71

/* Constants for register selection and masking */
#define RTC_NMI_INIT_VAL 0x8B
#define RTC_REG_A_V_MASK 0xF0
#define RTC_REG_B_V_MASK 0x40
#define RTC_REG_A_SELECT 0x8A
#define RTC_REG_C_SELECT 0x0C

/* Init constants */
#define RTC_SILENT       0
#define RTC_VERBOSE	 	 1
#define RTC_TEST         2

/* ioctl constants */
#define RTC_INIT         100
#define RTC_EOI          200
#define RTC_SET_FREQ     300


/* External functions */
void rtc_init(int mode);
void rtc_handle_interrupt(void);

#endif /* _RTC_H */
