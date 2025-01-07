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
    //Serial.println("light sleep");
    esp_sleep_enable_timer_wakeup(60000000);
    esp_light_sleep_start();
    //Serial.println("restarting the esp 32");
    ESP.restart();
    }
  client.loop();
  setupMQTT();
  sendData();
  delay(4000);
  Serial.println("deep sleep");
  esp_sleep_enable_timer_wakeup(600000000);
  esp_light_sleep_start();
  ESP.restart();
  //Serial.println("deep sleep");
 
}
 

// 1. Modularize the Code
// To enhance readability and maintainability, split the code into separate files for Wi-Fi, MQTT, DHT, and PMS functionalities.

// Actions:
// Create separate .h and .cpp files for the following:
// WiFiSetup: Handles Wi-Fi connection logic.
// MQTTClient: Handles MQTT connection and message publishing.
// DHTSensor: Reads temperature and humidity.
// PMSSensor: Integrates the PMS library and manages sensor readings.
// Benefits:
// Easier debugging and scalability.
// Focused development on individual components.
// 2. Refactor PMS Handling to Use the PMS Library
// Your comment mentions moving away from manual packet handling to the PMS library. This should simplify your PMS integration.

// Actions:
// Replace the readPMSData() function with library calls.
// Include support for sensor sleep/wake functionality.
// Example Refactor:
// cpp
// Copiar código
// #include <PMS.h>
// HardwareSerial pmsSerial(2); // Use hardware serial
// PMS pms(pmsSerial);
// PMS::DATA data;

// void setupPMSSensor() {
//   pmsSerial.begin(9600);
//   pms.wakeUp();
//   delay(30000); // Warm-up period
// }

// void readPMSData() {
//   if (pms.read(data)) {
//     pm1_0 = data.PM_AE_UG_1_0;
//     pm2_5 = data.PM_AE_UG_2_5;
//   } else {
//     Serial.println("Failed to read PMS data.");
//   }
// }

// void sleepPMSSensor() {
//   pms.sleep();
// }
// 3. Improve Wi-Fi Connection Logic
// There’s redundancy in your Wi-Fi connection code (setupCommunication and reconnect). Combine these to streamline functionality.

// Actions:
// Create a reusable function connectWiFi() that handles both initial connection and reconnections.
// 4. Refine MQTT Logic
// Your reconnect function handles both Wi-Fi and MQTT connections. This coupling can be separated.

// Actions:
// Create a dedicated MQTT setup and reconnection logic.
// Add QoS support to improve message delivery reliability.
// Example:
// cpp
// Copiar código
// void setupMQTT() {
//   client.setServer(mqtt_server, 1883);
//   client.setCallback(callback); // Optional: Add a callback for incoming messages
// }

// void reconnectMQTT() {
//   if (!client.connected()) {
//     while (!client.connected()) {
//       if (client.connect(clientID, username, passwd)) {
//         Serial.println("Connected to MQTT!");
//       } else {
//         Serial.print("MQTT connection failed, rc=");
//         Serial.print(client.state());
//         Serial.println(" retrying...");
//         delay(5000);
//       }
//     }
//   }
// }
// 5. Optimize NTP Integration
// Your NTP setup isn’t necessary if not used. If you want timestamps for MQTT messages, integrate them directly into your message logic.

// Actions:
// Remove unused NTP code.
// Add timestamps via the PMS library or a local RTC.
// 6. Use Conditional Compilation
// Your comments suggest using #ifdef for optional features. This is a great idea for hardware-dependent logic.

// Actions:
// Add preprocessor directives for PMS, DHT, and MQTT logic.
// cpp
// Copiar código
// #ifdef ENABLE_PMS
//   readPMSData();
// #endif

// #ifdef ENABLE_DHT
//   readDHTData();
// #endif
// 7. Add Error Handling and Logging
// Ensure robust handling for sensor failures, MQTT disconnections, and Wi-Fi dropouts.

// Actions:
// Add retries with backoff for Wi-Fi and MQTT.
// Log errors to the serial console or an MQTT diagnostic topic.
// 8. Test and Validate Each Module
// Before integration, test each module independently:

// Wi-Fi connection stability.
// MQTT publishing under different network conditions.
// PMS library integration with sleep/wake support.
// DHT sensor data accuracy.
// 9. Prepare for Deployment
// Once the system is stable:

// Document your code: Add clear comments for each module.
// Optimize for low power: Use sleep modes for both the ESP32 and sensors.
// Monitor Performance: Use MQTT to log sensor status and device uptime.
