#include "tg0_wifi.h"
#include <list>
#include <vector>
using namespace std;

// Replace these with your WiFi network settings
char* ssid = "ESP8266"; //replace this with your WiFi network name
char* password = "ESP8266Test"; //replace this with your WiFi network password
const int channel = 6;
IPAddress ip_addr, gateway;
const char* server_address = "192.168.1.180";
const char* gateway_address = "192.168.1.1";
IPAddress subnet(255, 255, 255, 0);
WiFiServer tg0_server(DATA_PORT);
WiFiClient* tg0_clients[MAX_CLIENTS] = { NULL };
byte buff[200] = {0};

void setup_server(char* ssid = ssid, char* password = password)
{/* You can remove the password parameter if you want the AP to be open. */
  
  WiFi.mode(WIFI_OFF);
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  boolean result = WiFi.softAP(ssid, password, channel);
  if(result==true) {
    Serial.println("wifi established!");
    if (ip_addr.fromString(server_address) && gateway.fromString(gateway_address)) {
        WiFi.softAPConfig(ip_addr, gateway, subnet);
        Serial.println("host configured!");
    }
  }
  tg0_server.begin();
}

void check_client() {
  WiFiClient client = tg0_server.available();
  if (client != NULL) {
    Serial.println(client.remoteIP());
  
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (tg0_clients[i] != NULL && tg0_clients[i]->remoteIP() == client.remoteIP()) {
        tg0_clients[i]->stop();
        delete tg0_clients[i];
        tg0_clients[i] = new WiFiClient(client);
        break;
      }
      else if (tg0_clients[i] == NULL) {
        Serial.print(" client number: "); Serial.print(i);
        tg0_clients[i] = new WiFiClient(client); 
        tg0_clients[i]->setTimeout(10);
        tg0_clients[i]->setNoDelay(true);
        break;
      }
    }
  }
  for (int i = 0; i < MAX_CLIENTS; i++) {
    Serial.print(tg0_clients[i]->connected());
    if (tg0_clients[i] != NULL && tg0_clients[i]->available() > 0) {
      int bytes_to_read = tg0_clients[i]->available();
      int returned_bytes = tg0_clients[i]->readBytes(buff, bytes_to_read);
      if (returned_bytes > 0) {
        Serial.print(" returned bytes "); Serial.println(returned_bytes);
        //Serial.write(buff, returned_bytes);
      } else {
       // Serial.println("this client is NULL!");
      }
    }
  } 
}
