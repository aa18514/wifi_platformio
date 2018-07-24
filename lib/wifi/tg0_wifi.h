#ifndef TG0_WIFI_H
#define TG0_WIFI_H

#if defined (ESP8266)
    #include <ESP8266WiFi.h>
#endif
#if defined (ESP32)
    #include <WiFi.h>
#endif

#include <Arduino.h>

#define DATA_PORT 8000
#define MAX_CLIENTS 4
extern void setup_server(char* ssid, char* password);
extern void check_client();
#endif
