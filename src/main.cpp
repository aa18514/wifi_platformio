#include "tg0_wifi.h"

void setup() {
  Serial.begin(115200);
  while(!Serial);
  setup_server("WHOAMI", "Xfojasifjasiofj?");
}

void loop() {
  check_client();
}