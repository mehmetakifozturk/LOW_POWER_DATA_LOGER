#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H
#include <SPI.h>
#include <Adafruit_MAX31865.h>
#include <definations.h>
#include <AT24C512.h>
#define RREF      430.0
#define RNOMINAL  100.0

class mainFunctions
{
private:
    /* data */
//SLEEP_RANGE_ADDRESS 100
float offset=0.00;//SENSOR_OFFSET_ADDRESS 125
uint16_t recordedDataSize=0;//RECORDED_DATA_SIZE_ADDRESS 105//EEPROM BEGINING ADDRES
float heat=0.00;

AT24C512 Eeprom;
Adafruit_MAX31865 Pt100 = Adafruit_MAX31865(2, 23, 19, 18);
public:
uint8_t sleepRange=1;
Device myDevice={"1708_21","PT100"};
   void begin(void);
   void read_Info(void);
   float read_Heat(void);// usb modu okuma işlemi
   void date_update(String currentTime );// everytime incrise minute as sleepRange
   String check_date(void);//
   void calculate_date(void);//EVERY TIME WHEN DEVICE WAKEUP DEVICE CALCULATE THE TIME
   void wanted_last_day_data_read(String dayToDay);
   void all_record_read(void);
   bool mesure_heat_keep_it(void);
   bool save_TheData(void);// ölçüm modu data kaydı fonklsiyonu
   void delete_Memory(void);
   void setOffset(String currentOffset);
   String check_the_comand(String incomingData);
   String check_the_value(String incomingData);
   void work_the_comand(String comand,String value);
   void setSleepRange(String range);
};




#endif