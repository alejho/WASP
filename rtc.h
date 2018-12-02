#ifndef RTC_H
#define RTC_H

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS1307.h>
#include "common.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

void rtc_init();
uint8_t rtc_day_now();
uint8_t rtc_hour_now();
uint8_t rtc_minute_now();
void rtc_print_date_time();

#endif
