#include "rtc.h"

RtcDS1307<TwoWire> Rtc(Wire);

void rtc_init(){

    Rtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    

	//set rtc datetime at first compile
    if (!Rtc.IsDateTimeValid()) {
        Rtc.SetDateTime(compiled);
    }
	//set running if it isn't
    if (!Rtc.GetIsRunning()){
        Rtc.SetIsRunning(true);
    }
    RtcDateTime now = Rtc.GetDateTime();
	//for some reason now is older than compile time
    if (now < compiled) {
        Rtc.SetDateTime(compiled);
    }
	// never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);

}

uint8_t rtc_day_now(){

    RtcDateTime l_now = Rtc.GetDateTime();
    return l_now.Day();
}

uint8_t rtc_hour_now(){

    RtcDateTime l_now = Rtc.GetDateTime();
    return l_now.Hour();
}

uint8_t rtc_minute_now(){

    RtcDateTime l_now = Rtc.GetDateTime();
    return l_now.Minute();
}

void rtc_print_date_time(){

    RtcDateTime l_now = Rtc.GetDateTime();
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            l_now.Month(),
            l_now.Day(),
            l_now.Year(),
            l_now.Hour(),
            l_now.Minute(),
            l_now.Second() );

    DBG_OUTPUT_PORT.println(datestring);        
}