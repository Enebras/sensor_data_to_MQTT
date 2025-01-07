#include <PMS.h>
#include <SoftwareSerial.h>

#define PMS5003_RX_PIN 14                                      
#define PMS5003_TX_PIN 26  
#define PMS5003_SET_PIN 16
float pm1_0;
float pm2_5;

SoftwareSerial PMSserial(PMS5003_RX_PIN ,PMS5003_TX_PIN);
PMS pms(PMSserial);
PMS::DATA data;

void readPMS(){
  Serial.println("Waking up, wait 30 seconds for stable readings...");
  
  Serial.println("Send read request...");
  pms.requestRead();
  delay(30000);

  Serial.println("Wait max. 1 second for read...");
  if (pms.readUntil(data))
  {
         Serial.print("PM 1.0 (ug/m3): ");
         Serial.println(data.PM_AE_UG_1_0);

         Serial.print("PM 2.5 (ug/m3): ");
         Serial.println(data.PM_AE_UG_2_5);

    pm1_0 = data.PM_AE_UG_1_0;
    pm2_5 = data.PM_AE_UG_2_5;
  }
  else
  {
    Serial.println("did'nt worked");
    pms.sleep();
    delay(1000);
    pms.wakeUp();
    delay(30000);
    pms.requestRead();
    delay(1000);
    if(!pms.readUntil(data)){
        pms.sleep();
        delay(1000);
        pms.wakeUp();
        delay(30000);
      for(int i =0; i <5; i++){
        pms.requestRead();
        delay(3000);
      }
       if(!pms.readUntil(data)){
      pm1_0 = 101;
      pm2_5 = 101;
    }
    }
    else{
      pm1_0 = data.PM_AE_UG_1_0;
      pm2_5 = data.PM_AE_UG_2_5;
    }
   

  }
  pms.sleep();
  digitalWrite(PMS5003_SET_PIN, LOW);
}

void setupPMS()
{
  pinMode(PMS5003_SET_PIN, OUTPUT);
  digitalWrite(PMS5003_SET_PIN, HIGH);
  PMSserial.begin(9600);
    while (PMSserial.available()) {
    PMSserial.read(); // Discard any remaining bytes in the buffer
  }
  pms.passiveMode();  
  delay(30000);
  pms.requestRead();
}



