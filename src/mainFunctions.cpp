#include "mainFunctions.h"

void mainFunctions::begin()
{   
    Eeprom.begin(4); 
    
    Eeprom.pul(SLEEP_RANGE_ADDRESS,sleepRange);
     
    Eeprom.pul(SENSOR_OFFSET_ADDRESS,offset);
    Eeprom.pul(RECORDED_DATA_SIZE_ADDRESS,recordedDataSize);
    Pt100.begin(MAX31865_3WIRE);// max ic begin
    pinMode(USB_WAKEUP_PIN,INPUT);// usb pin def
    pinMode(POWER_WAKEUP_PIN,OUTPUT); // wwakeup pin def
    digitalWrite(POWER_WAKEUP_PIN,HIGH);// wake up pin status
    Eeprom.pul(SLEEP_RANGE_ADDRESS,sleepRange);
}

void mainFunctions::read_Info(){
   // Eeprom.put(INFO_SPACE_STARTING_ADDRESS,myDevice);
    Eeprom.pul(INFO_SPACE_STARTING_ADDRESS,myDevice);
  Serial.print("serial_number: ");Serial.println(myDevice.serial_number);
     Serial.print("sleepRange: ");Serial.println(sleepRange);
      Serial.print("sensorType: ");Serial.println(myDevice.sensorType);
       Serial.print("recordedDataSize: ");Serial.println(recordedDataSize);
        Serial.print("lastMemoryAddress: ");Serial.println(recordedDataSize*4);
          String date="HOUR "+String(myDevice.date[1]) + ":" + String(myDevice.date[0]) + " DATE " + String(myDevice.date[2]) + ":" + String(myDevice.date[3])+ ":20" +String(myDevice.date[4]);
          Serial.println(date);
          Serial.print("offset: "); Serial.println(offset);  
          Serial.print("HEATH: "); Serial.print(read_Heat());Serial.println("°C");
          Serial.println("-----KOMUTLAR----");
          Serial.println("DELETE=0  HAFIZA ADRESINI SIFIRLAR");
          Serial.println("READALL=0 TÜM KAYIDI DOKER ");
          Serial.println("DATE=XX.XX.XX.XX.XX   SAATI GUNCELLER");
          Serial.println("READONE=XX  SON X. GUNU DOKER");
          Serial.println("SETOFFSET=X.Y +/- SENSOR OFSETINI GIRER");
          Serial.println("SETSLEEP=XX  1 - 120 DK ARASI UYKU ARALIGI GIRER");
          Serial.println("READINFO=XX  CIHAZ INFOSUNU DOKER");

}

float mainFunctions::read_Heat(){
  
    return Pt100.temperature(RNOMINAL, RREF)+offset;
    
}

void mainFunctions::date_update(String currentTime ){
    String hour="XX";String minute="XX";String day="XX";String month="XX";String year="XX";
    Serial.println("bygy :" + currentTime);
    unsigned int i_partOfTime=0;
    uint8_t b_partOfTime=0;
     hour[0]=currentTime[0];
     hour[1]=currentTime[1];
      i_partOfTime=hour.toInt();
      b_partOfTime=i_partOfTime;
      myDevice.date[1]=b_partOfTime;
     minute[0]=currentTime[3];
     minute[1]=currentTime[4];
     i_partOfTime=minute.toInt();
      b_partOfTime=i_partOfTime;
      myDevice.date[0]=b_partOfTime;
     day[0]=currentTime[6];
     day[1]=currentTime[7];
     i_partOfTime=day.toInt();
      b_partOfTime=i_partOfTime;
     myDevice.date[2]=b_partOfTime;
     month[0]=currentTime[9];
     month[1]=currentTime[10];
     i_partOfTime=month.toInt();
      b_partOfTime=i_partOfTime;
     myDevice.date[3]=b_partOfTime;
     year[0]=currentTime[12];
     year[1]=currentTime[13];
      Serial.println("yeah :" + year);
      i_partOfTime=year.toInt();
      b_partOfTime=i_partOfTime;
     myDevice.date[4]=b_partOfTime;
     myDevice.timeSetDataNumber=recordedDataSize;
     Eeprom.put(INFO_SPACE_STARTING_ADDRESS,myDevice);
    
     String nawDate=String(myDevice.date[1]) + ":" + String(myDevice.date[0]) + ":" + String(myDevice.date[2]) + ":" + String(myDevice.date[3])+ ":20" +String(myDevice.date[4]);
    
     Serial.println("CURRENT TIME IS = "+nawDate);
}

void mainFunctions::calculate_date(){
   myDevice.date[0]+=sleepRange;
   if(myDevice.date[0]>59){
       myDevice.date[0]=myDevice.date[0]%60;
       myDevice.date[1]++;
     if(!(myDevice.date[1]%24)){
        myDevice.date[1]=0;
        myDevice.date[2]++;
              if(!(myDevice.date[2]%31)){
                   myDevice.date[2]=1;
                   myDevice.date[3]++;
                     if(!(myDevice.date[3]%13)){
                        myDevice.date[3]=1;
                        myDevice.date[4]++;
                   
                      }
              }
     }
   }
   
}




bool mainFunctions::mesure_heat_keep_it(){
   heat=read_Heat();
   uint32_t address=(recordedDataSize*4)+DATA_SPACE_STARTING_ADDRESS;
if(Eeprom.put(address,heat)){
  recordedDataSize++;
  return Eeprom.put(RECORDED_DATA_SIZE_ADDRESS,recordedDataSize);
}else{
    return 0;
}
   
}
   void mainFunctions::all_record_read(void){
     Eeprom.pul(INFO_SPACE_STARTING_ADDRESS,myDevice);
    float reading_heat=0;
    uint32_t adrss=0;
    for(unsigned int dataNumber=0;dataNumber<recordedDataSize;dataNumber++){
     calculate_date();
     String date="H "+String(myDevice.date[1]) + ":" + String(myDevice.date[0]) + " D " + String(myDevice.date[2]) + ":" + String(myDevice.date[3])+ ":20" +String(myDevice.date[4]);
     adrss=(dataNumber*4)+DATA_SPACE_STARTING_ADDRESS;
     Eeprom.pul(adrss,reading_heat);    
      date+=" R: "+String(reading_heat)+"°C";
      Serial.println(date);
      if(Serial.available()>0)break;
     
    }
   
   }
   void mainFunctions::delete_Memory(){
    recordedDataSize=0;
    Eeprom.put(RECORDED_DATA_SIZE_ADDRESS,recordedDataSize);
     Serial.println("MEMORY STARTING POINT IS ZERO");
   }
   
   
   void mainFunctions::wanted_last_day_data_read(String dayToDay){
     Eeprom.pul(INFO_SPACE_STARTING_ADDRESS,myDevice);
      int wantedDay=dayToDay.toInt();
      if((wantedDay*(MINUTE_OF_ONE_DAY/sleepRange))>recordedDataSize){
        Serial.print("ERROR!! RECORD IS ENOUGHT !!! ");
      }else{
        
      float reading_heat=0;
      uint32_t adrss=0;
      unsigned int beginningDataNumber= recordedDataSize-(wantedDay*(MINUTE_OF_ONE_DAY/sleepRange));
      for(unsigned int dataNumber=0;dataNumber<beginningDataNumber;dataNumber++){
      calculate_date(); 
     }

     for(unsigned int dataNumber=beginningDataNumber;dataNumber<(beginningDataNumber+(MINUTE_OF_ONE_DAY/sleepRange));dataNumber++){
     calculate_date();
     String date="H "+String(myDevice.date[1]) + ":" + String(myDevice.date[0]) + " D " + String(myDevice.date[2]) + ":" + String(myDevice.date[3])+ ":20" +String(myDevice.date[4]);
     adrss=(dataNumber*4)+DATA_SPACE_STARTING_ADDRESS;
     Eeprom.pul(adrss,reading_heat);    
     Serial.print(date); Serial.print(" RECORD: "); Serial.print(reading_heat); Serial.println("°C");
     
    }
      }

   }

   void mainFunctions::setOffset(String currentOffset){
      float fl_currentOffset=currentOffset.toFloat();
      if(fl_currentOffset<-50.00||fl_currentOffset>50.00){
         Serial.print("ERROR!! value does not make sense !! ");
      }else{
        offset=fl_currentOffset;
         Serial.print("NEW OFFSET VALUE IS TO ");
          Serial.print(offset);
          Serial.print(" °C");
        Eeprom.put(SENSOR_OFFSET_ADDRESS,offset);
      }
       
   }
  
  

    String mainFunctions::check_the_comand(String incomingData){
     int i=incomingData.length();

while(incomingData[i-1]!='='){
  incomingData[i-1]=NULL;
  i--;
    if(i<-100){
               return "DEFAULT";
                break;
               }
}
incomingData[i-1]=NULL;

     return incomingData;
   }

            String mainFunctions::check_the_value(String incomingData){
            int i=0;
            while(incomingData[i]!='='){
             incomingData[i]=NULL;
              i++;
               if(i>100){
                incomingData="0";
                return "0";
                break;
               }
            }
            int j=incomingData.length();
            i++;
            j=j-i;
            char charArray[j];
            j=0;
            
            while(incomingData[i]!='\n'){
            charArray[j]=incomingData[i];
              i++;
              j++;
              
            }
              charArray[j]='\n';
           incomingData=charArray;
           
           return incomingData;
           }



           void mainFunctions::setSleepRange(String range){
             int i_range=range.toInt();
             uint8_t b_range=i_range;
              if(b_range>120||b_range<1){
         Serial.print("ERROR!! TIME IS SO LONG OR SO SHORT !! ");
      }else{
         sleepRange=b_range;
          Serial.print("NEW SLEEP RANGE IS TO ");
          Serial.print(sleepRange);
          Serial.print(" MINUTE");
        Eeprom.put(SLEEP_RANGE_ADDRESS,sleepRange);
      }

           }

            void mainFunctions::work_the_comand(String comand,String value){
                    
                    if(comand=="DELETE"){
                           delete_Memory();
                    }else if(comand=="DATE"){
                           date_update( value);
                    }
                   else if(comand=="READONE"){
                              wanted_last_day_data_read(value);
                    }else if(comand=="SETSLEEP"){
                           setSleepRange(value);
                    }else if(comand=="SETOFFSET"){
                              setOffset(value);
                    }else if(comand=="READALL"){
                          all_record_read();
                    }else if(comand=="READINFO"){
                           read_Info();
                    }else{
                      Serial.println("COMAND NOT AVAILABLE !!");
                      read_Info();
                    }
            }

  