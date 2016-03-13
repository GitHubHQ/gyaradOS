/* rtc.h - Defines for real time clock functions
 */
#ifndef _RTC_H
#define _RTC_H

#include "lib.h"

#define RTC_REG_NUM_PORT 0x70
#define RTC_RW_CMOS_PORT 0x71

#define RTC_NMI_INIT_VAL 0x8B
#define RTC_REG_A_V_MASK 0xF0
#define RTC_REG_B_V_MASK 0x40
#define RTC_REG_A_SELECT 0x8A
#define RTC_REG_C_SELECT 0x0C

void rtc_init();

void rtc_special_eoi();

#endif /* _RTC_H */
