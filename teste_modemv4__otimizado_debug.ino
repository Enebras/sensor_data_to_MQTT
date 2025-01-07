#include "Communication.h"
unsigned long updateInterval = 600000;//10 minutes
unsigned long lastMsg = 2000 - updateInterval;// at future "if (now - lastMsg > updateInterval)" so update interval will be set to 
                                              // a large integer because -- = + "now - lastMsg = 1000 - (-598000) = 1000 + 598000 = 599000" 

void setup() {
  Serial.begin(9600);
  //starts the usb hardwareserial communication 
   
  setupPMS();
  setupDHT();
  Serial.println("teste_modemV4");
}

void loop() {
  Serial.print("begning");
  delay(1000);
  readPMS();
  readDHT();
  if(!setupWiFi()){
    Serial.println("light sleep");
    esp_sleep_enable_timer_wakeup(60000000);
    esp_light_sleep_start();
    Serial.println("restarting the esp 32");
    ESP.restart();
    }
  client.loop();
  setupMQTT();
  sendData();
  delay(4000);
  Serial.println("light sleep");
  esp_sleep_enable_timer_wakeup(600000000);
  esp_light_sleep_start();
  ESP.restart();
  Serial.println("this will not be printed");
 
}
 


