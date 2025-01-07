#include <DHT.h>
#define DHTPOWERPIN 32
#define DHT11PIN 33

DHT dht(DHT11PIN, DHT11);//implement #ifdef DHT11PIN then run it 

float temperature, humidity;

void readDHT() {//function to read DHT data 
  digitalWrite(DHTPOWERPIN, HIGH);
  delay(3000);
  temperature = dht.readTemperature();//simple and fuctional temperature and humidity reading 
  humidity = dht.readHumidity();

  if (isnan(temperature) && isnan(humidity)) {
    for(int i = 0; i < 5; i++){
        if (!isnan(temperature) && !isnan(humidity)) {
        break;
        }
      delay(500);
      temperature = dht.readTemperature();//simple and fuctional temperature and humidity reading 
      humidity = dht.readHumidity();
    }
    
  }

   
   

  pinMode(DHT11PIN,OUTPUT);
  digitalWrite(DHT11PIN, LOW);
  digitalWrite(DHTPOWERPIN, LOW);

}

void setupDHT(){
  pinMode(DHTPOWERPIN, OUTPUT);
  dht.begin();
  
}


