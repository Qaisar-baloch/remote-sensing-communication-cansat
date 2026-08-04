#ifndef SENSORS_H
#define SENSORS_H
#include <LoRa.h>
#include "SoftwareSerial.h"
#include <WiFi.h>

extern bool intr;                      // Definition of bool for global access
extern String CSV_STRING;              // Defintion of String for global access
extern byte CAM_byte;                  // Definition of Byte for global access
extern bool chute_release;             // Definition of parachute release flag
extern float altitude;
extern hw_timer_t *Timer0_Cfg;         // Definition of Timer 0 configuration

extern EspSoftwareSerial::UART ss;            // Used to define swSer
extern HardwareSerial cam_s;

// Function Definitions
void get_bmp_280();
void get_mpu6050();
void getTemp102();
void mpu6050_setup();
void bmp280_setup();
void timer_setup();
void LORA_SETUP();
void IRAM_ATTR onTimer();
void get_gps();
void get_RTC();
void RTC_setup();
void downloadImage(const char* url, const char* filePath);
void test();




#endif