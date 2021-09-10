#ifndef DEFINES_H
#define DEFINES_H
#include <Arduino.h>
#include <stdlib.h>
#include <stdint.h>
#define DEVICE_ID 50821-1//DEVICE ID NUMBER AS STRING BUT UNCHANGEABLE 
#define INFO_SPACE_SIZE 144 //as byte
#define INFO_SPACE_STARTING_ADDRESS 0//EEPROM BEGINING ADDRES
#define SLEEP_RANGE_ADDRESS 100
#define RECORDED_DATA_SIZE_ADDRESS 105//EEPROM BEGINING ADDRES
#define SENSOR_OFFSET_ADDRESS 125
#define DATA_SPACE_STARTING_ADDRESS 145//EEPROM BEGINING ADDRES
#define SIZE_OF_ONE_DATA 4
#define MEMORY_SIZE 262000// EEPROM SIZE AS BYTE
#define RECORD_SIZE_CAPACITY 2222//byte bölünecek
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  2
#define USB_WAKEUP_PIN 32
#define POWER_WAKEUP_PIN 33
#define MINUTE_OF_ONE_DAY 1140




 typedef struct{
String serial_number;     
String sensorType;
uint8_t date[5];// d.m.h.m       day/mount/hour/minute
uint16_t timeSetDataNumber;
}Device;







#endif