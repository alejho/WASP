#include "ntp.h"

int8_t timeZone = 1;
int8_t minutesTimeZone = 0;
String ntpServer = "pool.ntp.org";

bool wifiFirstConnected = false;
uint8_t ntp_day = 0;
uint8_t ntp_hour = 0;
uint8_t ntp_minute = 0;
bool ntp_ok = false;

void onSTAConnected (WiFiEventStationModeConnected ipInfo) {
    DBG_OUTPUT_PORT.printf ("Connected to %s\r\n", ipInfo.ssid.c_str ());
}

// Start NTP only after IP network is connected
void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
    DBG_OUTPUT_PORT.printf ("Got IP: %s\r\n", ipInfo.ip.toString ().c_str ());
    DBG_OUTPUT_PORT.printf ("Connected: %s\r\n", WiFi.status () == WL_CONNECTED ? "yes" : "no");
    digitalWrite (ONBOARDLED, LOW); // Turn on LED
    wifiFirstConnected = true;
}

// Manage network disconnection
void onSTADisconnected (WiFiEventStationModeDisconnected event_info) {
    DBG_OUTPUT_PORT.printf ("Disconnected from SSID: %s\n", event_info.ssid.c_str ());
    DBG_OUTPUT_PORT.printf ("Reason: %d\n", event_info.reason);
    digitalWrite (ONBOARDLED, HIGH); // Turn off LED
    //NTP.stop(); // NTP sync can be disabled to avoid sync errors
}

void processSyncEvent (NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
        DBG_OUTPUT_PORT.print ("Time Sync error: ");
        if (ntpEvent == noResponse)
            DBG_OUTPUT_PORT.println ("NTP server not reachable");
        else if (ntpEvent == invalidAddress)
            DBG_OUTPUT_PORT.println ("Invalid NTP server address");
    } else {
        //DBG_OUTPUT_PORT.print ("Got NTP time: ");
        //DBG_OUTPUT_PORT.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
    }
}

boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event


void ntp_init(){

    static WiFiEventHandler e1, e2, e3;
    NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
        ntpEvent = event;
        syncEventTriggered = true;
        //DBG_OUTPUT_PORT.println("onNTPSyncEvent");
    });

    e1 = WiFi.onStationModeGotIP (onSTAGotIP);// As soon WiFi is connected, start NTP Client
    e2 = WiFi.onStationModeDisconnected (onSTADisconnected);
    e3 = WiFi.onStationModeConnected (onSTAConnected);

}

bool ntp_available(){

    return ntp_ok;
}

uint8_t ntp_day_now(){

    return ntp_day;
}

uint8_t ntp_hour_now(){

    return ntp_hour;
}

uint8_t ntp_minute_now(){

    return ntp_minute;
}

void ntp_refresh(){

    if (wifiFirstConnected) {
        wifiFirstConnected = false;
        NTP.begin (ntpServer, timeZone, true, minutesTimeZone);
        NTP.setInterval (63);
    }

    if (syncEventTriggered) {
        //DBG_OUTPUT_PORT.println("syncEventTriggered");
        processSyncEvent (ntpEvent);
        syncEventTriggered = false;
    }
    delay(0);

    static int last = 0;

    if ((millis () - last) > NTP_REFRESH_INTERVAL) {
        
        time_t l_now = NTP.getTime();
        ntp_day = day(l_now);
        ntp_hour = hour(l_now);
        ntp_minute = minute(l_now);
        //DBG_OUTPUT_PORT.println("ntp time refreshed!");
        last = millis();

        if (WiFi.isConnected() && NTP.getTime() != 0){
            ntp_ok = true;
        }
        else{
            ntp_ok = false;
        }

    }
    delay (0);
}

void ntp_print_date_time(){

    DBG_OUTPUT_PORT.println(NTP.getTimeDateString());
}
