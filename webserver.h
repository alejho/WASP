#ifndef WASP_H
#define WASP_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <ArduinoJson.h>

#include "common.h"
#include "relay.h"
#include "rules.h"
#include "ntp.h"

void srv_listFiles();
void srv_init();
bool srv_handle_file_read(String x_path);
void srv_handle_client();

#endif

