#ifndef NTP_H
#define NTP_H

#include "common.h"
#include <NtpClientLib.h>
#include <ESP8266WiFi.h>


#define ONBOARDLED 2 

void ntp_init();

bool ntp_available();

uint8_t ntp_day_now();
uint8_t ntp_hour_now();
uint8_t ntp_minute_now();

void ntp_refresh();
void ntp_print_date_time();


#endif
