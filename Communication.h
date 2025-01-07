#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "DHT_ctrl.h"
#include "PMS_ctrl.h"

const char* ssid = "romeu";  // maintain this pointer to the ssid field from wifi library
const char* password = "1234567890";
const char* mqtt_server = "iota.vimacsolucoes.com.br";
const char* ntpServer = "time.google.com";  // for instance this part is'nt needed
const char* gmtOffset = "GMT+3";            //this also
const char* mqttTopic = "enebras/b2549fac-7752-4f05-83d9-5354ca17856a";
const char* clientID = "b2549fac-7752-4f05-83d9-5354ca17856a";
const char* username = "enebras";
const char* passwd = "b2549fac-7752-4f05-83d9-5354ca17856a";


String datetime, mac;  // sets two variables, one for store the datetime from the NTP server and other to store the mac adress
                      // from your device in the wifi network( it will be used to conect with the mqtt server)

WiFiClient espClient;// creates an instance of the Wificliente class at the network so you will have an object to handle TCP connection
PubSubClient client(espClient); //creates an instance client of the pubSubclient class with the parameters from espClient to handle 


#define MSG_BUFFER_SIZE (400)// the size of the payload 
char msg[MSG_BUFFER_SIZE]; //the array to store the payload

bool sendData() {//function to send data to MQTT
       Serial.print("PM 1.0 (ug/m3): ");
       Serial.println(pm1_0);

       Serial.print("PM 2.5 (ug/m3): ");
       Serial.println(pm2_5);
  DynamicJsonDocument doc(MSG_BUFFER_SIZE);// creates a json document with the size of the msg buffer
  doc["device"] = WiFi.macAddress();//creates the field in the json document
  doc["temperature"] = String(temperature);
  doc["humidity"] = String(humidity);
  doc["pm1_0"] = String(pm1_0);
  doc["pm2_5"] = String(pm2_5);

  serializeJson(doc, msg);//serializes the msg using the json document convert the array in a json doc

  if(!client.publish(mqttTopic, msg)){
    return false;
    Serial.println("did'nt sended any data");
  }//publish the msg to the MQTT server

  Serial.print("Publish message: ");//printa a mensagem
  Serial.println(msg);
  return true; 
}

bool reconnectMQTT() {//function to reconnect with the MQTT server
if(WiFi.status() == WL_CONNECTED){
  if (!client.connected()) {//check for connection with MQTT server
    unsigned long mqttStartAttemptTime = millis();                            // Separate timer for MQTT connectio attempt
    while (!client.connected() && millis() - mqttStartAttemptTime < 10000) {  // 10-second timeout for MQTT( a loop that makes a 
     //serie of requests to the MQTT server)
      //Serial.print("Attempting MQTT connection...");
      if (client.connect(clientID, username, passwd)) {//send those credentials if mqtt accepts returns true 
        //Serial.println("connected to MQTT");
        return true;
      } 
      else {
          // Serial.print("failed, rc=");
          // Serial.print(client.state());//returns a integer for diagnose why is'nt connecting with the MQTT server
          // Serial.println(" retrying in 5 seconds");
        delay(5000);
        
      }
    }
    //Serial.println("MQTT reconnection failed after timeout.");
    return false;
  }
  else{
    return true;
  }
}
}

bool setupMQTT(){
  client.setServer(mqtt_server, 1883);
  if (WiFi.status() == WL_CONNECTED){
    if (!client.connected()) {
      return reconnectMQTT();
      }
    }
    else{
      return true;
    }   
  }



bool setupWiFi() {//this goes in the setup
if(WiFi.status() != WL_CONNECTED){
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);//set the with to station mode 
  WiFi.begin(ssid, password);// tells wich network to connect(it will keep running the whole program,  it only makes sense to run 
   //the code if with is connected)

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 60000) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {//same logic two times 
      Serial.println("WiFi connected");
    //  Serial.println("IP address: ");
    //  Serial.println(WiFi.localIP());
    return true;
    // just set the client server and the port to connect
  }
  if(WiFi.status() != WL_CONNECTED){
    return false;
  }
}
}

bool reconnectWiFi(){//this function will be used before send the data 

  if (WiFi.status() != WL_CONNECTED) {//checks if the wifi status is disconected
    //Serial.print("Reconnecting to WiFi...");
    WiFi.begin(ssid, password);//try to conect with the wifi, it's a asyncronous loop method that dont stop(
      // the program should run only if the wifi is connected )

    unsigned long startAttemptTime = millis();//starts to count the time it's tryng to connect
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) { 
      delay(500);
      //Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {// run the code
     // Serial.println("Reconnected to WiFi!");
      return true;
    } else {
     // Serial.println("WiFi reconnection failed.");//keep retryng for a period of time and then restart
      return false;  // Skip MQTT reconnection if WiFi fails( returns nothing)
    }
  }
}


