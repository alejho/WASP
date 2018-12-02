#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

#define DBG_OUTPUT_PORT Serial
//rules will be checked each CHECK_RULES_INTERVAL ms..this should be much lesser than 60s!
#define CHECK_RULES_INTERVAL 5000 
//current time refresh 
#define NTP_REFRESH_INTERVAL 5000 

#define RULES_ADD_OK 0
#define RULES_ADD_MAX_RULES 1
#define RULES_ADD_ALREADY_EXISTS 2
#define RULES_ADD_LOAD_FAILED 3


#endif
