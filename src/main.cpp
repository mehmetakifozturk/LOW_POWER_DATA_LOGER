
//#include <Arduino.h>
#include "mainFunctions.h"
mainFunctions functions;



void setup() {

//functions.date_update(19,10,14,8,21);
functions.begin();

 if(digitalRead(32)){
// usb takılıysa
Serial.begin(115200);//seri iletişimi başlat
if(digitalRead(32)){
functions.read_Info();
String incomingData;
String comand;
String value;
while(digitalRead(32)){
if(Serial.available()>0){
      incomingData=Serial.readString();
     
      comand=functions.check_the_comand(incomingData);
      value=functions.check_the_value(incomingData);
      Serial.println("COMAND IS "+  comand);
      Serial.println("VALUE IS "+  value);
      functions.work_the_comand(comand,value);
}

}

}

 }
 functions.mesure_heat_keep_it();
 esp_sleep_enable_timer_wakeup( functions.sleepRange * uS_TO_S_FACTOR*60);
 esp_deep_sleep_start();
 
 
}

void loop() {
 delay(1000);
 

 
}