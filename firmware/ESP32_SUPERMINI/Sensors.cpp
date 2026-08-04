//Includes
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include "Sensors.h"
#include <Arduino.h>
#include <LoRa.h>
#include "SoftwareSerial.h"
#include <TinyGPSPlus.h>
#include <ESP32Time.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
//Definitions
#define PIN_LORA_COPI         6
#define PIN_LORA_CIPO         5
#define PIN_LORA_SCK          4
#define PIN_LORA_CS           7
#define PIN_LORA_RST          3
#define PIN_LORA_DIO0         2
#define LORA_FREQUENCY        433E6


// Objects and namespaces
using namespace std;
Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;
TinyGPSPlus gps;
ESP32Time rtc;


// Variable declarations
static const int RXPin = 21, TXPin = 20;
int bmp280_en = 1;
int mpu6050_en = 1;
int value =0;
float reference;
float altitude = 0;
bool intr = 0;                    // bool for defining if interrupt has occured
bool chute_release = 0;           // bool to release parachute
int accel_z = 0;
hw_timer_t *Timer0_Cfg = NULL;
String CSV_STRING;
float roll;
float pitch;



// ******************************LORA_SETUP********************************************//
void LORA_SETUP(){
  
  LoRa.setPins (PIN_LORA_CS, PIN_LORA_RST, PIN_LORA_DIO0);
  LoRa.setSPIFrequency (20000000);
  LoRa.setTxPower (20);
    while (!LoRa.begin (LORA_FREQUENCY)) {
     Serial.println ("Starting LoRa failed!");
                                       }
    Serial.println(LoRa.begin(LORA_FREQUENCY));
}

// ******************************BMP280********************************************//
void   get_bmp_280(){
  if (bmp280_en ==1){
  float temp = bmp.readTemperature();
  float value = bmp.readPressure();
  float pressure = value;//101325;
  float al_value = bmp.readAltitude(1013.25);//1013.25
  altitude = al_value - reference;

  //Store the sensor values in the CSV string

  CSV_STRING += temp;
  CSV_STRING += ',';
  CSV_STRING += pressure;
  CSV_STRING += ',';
  CSV_STRING += altitude;
  CSV_STRING += ',';

  }
  else
  {
    Serial.println("BMP280 NOT FOUND");
  }
}
// ******************************BMP280********************************************//


// ******************************MPU6050********************************************//
void get_mpu6050(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Store the values in the CSV string
  accel_z = a.acceleration.z;
  roll = atan2(a.acceleration.y,a.acceleration.z)*180/PI;
  pitch = atan2(-a.acceleration.x,sqrt(a.acceleration.y*a.acceleration.y+a.acceleration.z*a.acceleration.z))*180/PI;
  CSV_STRING += roll;
  CSV_STRING += ',';
  CSV_STRING += pitch;
}
// ******************************MPU6050********************************************//
//Setup Functions

// ******************************MPU6050 SETUP********************************************//
void mpu6050_setup(){

  // Try to initialize!
  if (!mpu.begin()) {
    mpu6050_en = 0;
    Serial.println("mpu failed");
}
    mpu6050_en = 1;

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    break;
  case MPU6050_RANGE_4_G:
    break;
  case MPU6050_RANGE_8_G:
    break;
  case MPU6050_RANGE_16_G:
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    break;
  case MPU6050_RANGE_500_DEG:
    break;
  case MPU6050_RANGE_1000_DEG:
    break;
  case MPU6050_RANGE_2000_DEG:
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    break;
  case MPU6050_BAND_184_HZ:
    break;
  case MPU6050_BAND_94_HZ:
    break;
  case MPU6050_BAND_44_HZ:
    break;
  case MPU6050_BAND_21_HZ:
    break;
  case MPU6050_BAND_10_HZ:
    break;
  case MPU6050_BAND_5_HZ:
    break;
  }

  delay(100);

}
// ******************************MPU6050********************************************//

// ******************************BMP280 SETUP********************************************//
void bmp280_setup(){
  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    bmp280_en = 0;
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X16,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_250); /* Standby time. */
    reference = bmp.readAltitude(1013.25);
}
// ******************************BMP280********************************************//

// ******************************TimerSetup********************************************//
void timer_setup(){
    Timer0_Cfg = timerBegin(1000000); // 100 Hz Frequency
    timerAttachInterrupt(Timer0_Cfg, &onTimer); // Attach the interrupt with the ISR function
    timerWrite(Timer0_Cfg, 0);
    timerAlarm(Timer0_Cfg, 1000000, true, 0); // Enable the timer alarm
}
// ******************************TimerSetup********************************************//

// ******************************ISR for Timer Interrupt********************************************//
void ARDUINO_ISR_ATTR onTimer() {
  // This function will be called every 1 second
  intr = 1;
  if (altitude <= 300 && accel_z == 9 ){
    chute_release = 1;
  }
}

// ******************************GPS********************************************//
void get_gps(){
  gps.encode(ss.read());
  CSV_STRING += String(gps.location.lat() , 6);
  CSV_STRING += ',';
  CSV_STRING += String (gps.location.lng() , 6);
  CSV_STRING += ',';
  CSV_STRING += gps.location.isUpdated();
  CSV_STRING += ',';

}
// ******************************RTC_SETUP********************************************//
void RTC_setup(){
rtc.setTime(0, 0, 0, 17, 1, 2024);
}

// ******************************RTC********************************************//
void get_RTC(){
  
  CSV_STRING += rtc.getTime();
  CSV_STRING += ',';

}

// ******************************DOWNLOAD_IMAGE********************************************//
void downloadImage(const char* url, const char* filePath) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        File file = SPIFFS.open(filePath, FILE_WRITE);
        if (!file) {
            Serial.println("Failed to open file for writing");
            return;
        }

        http.writeToStream(&file);
        file.close();
        Serial.println("Image downloaded and saved to SPIFFS");
    } else {
        Serial.printf("Failed to download image, error: %d\n", httpCode);
    }
    http.end();
}

// ******************************BMP180********************************************//

void test(){
  if (accel_z > -5){
    cam_s.write('Z');
  }


}
