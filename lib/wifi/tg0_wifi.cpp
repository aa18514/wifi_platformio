#include "tg0_wifi.h"

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
const boolean is_ascii = true;

void setup_server(char* ssid = ssid, char* password = password)
{/* You can remove the password parameter if you want the AP to be open. */
  //WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_AP);
  boolean result = WiFi.softAP(ssid, password, channel);
  //while(!result) {
    //Serial.println("trying to connect!");
    //  result = WiFi.softAP(ssid, password, channel);
  //}
  Serial.println(result);
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
  if (client) {
  
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (tg0_clients[i] != NULL && tg0_clients[i]->remoteIP() == client.remoteIP()) {
          tg0_clients[i]->stop();
          delete tg0_clients[i];
          tg0_clients[i] = new WiFiClient(client);
          break;
      }
      if (tg0_clients[i] == NULL) {
        tg0_clients[i] = new WiFiClient(client); 
        tg0_clients[i]->setTimeout(10);
        tg0_clients[i]->setNoDelay(true);
        Serial.print(tg0_clients[i]->remoteIP());
        break;
      }
    }
  }
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (tg0_clients[i] != NULL){
      int bytes_to_read = tg0_clients[i]->available();
      if (tg0_clients[i] != NULL && bytes_to_read > 0) {
        int returned_bytes = tg0_clients[i]->readBytes(buff, bytes_to_read);
        if (returned_bytes > 0) {
          Serial.write(buff, returned_bytes);
        } 
      }
    } 
  }
}