#include "Sensors.h"                     
#define SW_TX                 20
#define SW_RX                 21

EspSoftwareSerial::UART ss;            // Used to define swSer
HardwareSerial cam_s(0);

const char* serverUrl = "http://192.168.4.1/image.jpg";  // URL for the image
static const int RXPin = 11, TXPin = 10;
int packet = 0;
byte take_pic = 'A';
byte transmit = 'C';
byte Log = 'B';
bool first = 1;
int analog;
float volts;

void setup() {
  Serial.begin(115200);               // Only to be executed if debugging
  cam_s.setPins(RXPin,TXPin);       // Set Pins for RX , TX for communication with CAM
  cam_s.begin (250000);             // Initialize Serial for GPS communication
  ss.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, SW_RX, SW_TX );  // Initialize Software Serial for communication with ESP_CAM
  mpu6050_setup();                    // Initialize the mpu6050 Sensor
  bmp280_setup();                     // Initialize the bmp280 Sensor
  //bmp_180.begin();
  LORA_SETUP();                       // Initialize LoRa
  delay(500);
  RTC_setup();                        // Initialize RTC
  timer_setup();                      // Initialize the timer to send an interrupt every 1 Second to raise the intr flag

  // WiFi.begin(ssid, password);         // Initialize WiFi

  // while (WiFi.status() != WL_CONNECTED) {
  //       delay(1000);
  //       Serial.println("Connecting to WiFi...");
  //  }
  //  Serial.println("Connected");

}


void loop() {

// If 1 second has passed then send the string if not then update the string
if (intr == 1){
  /*
                          CSVString Format: packet,RTC_time,gps_latitude,gps_longitude,gps_speed,bmp_temp,bmp_pressure,bmp_altitude,roll,pitch;
  */
    get_RTC();
    CSV_STRING += packet;
    CSV_STRING += ',';
    get_gps();
    get_bmp_280();                    // Add the values of bmp280 to CSV_STRING
   // get_bmp_180();                    // Add the values of bmp180 to CSV_STRING
    get_mpu6050();                    // Add the values of mpu6050 to CSV_STRING
    analog = analogRead(1);
    volts = analog * (3.3/4096);
    CSV_STRING += ',';
    CSV_STRING += volts;
  LoRa.beginPacket();
    LoRa.print (CSV_STRING);          // Send the CSV_STRING
  LoRa.endPacket();
  CSV_STRING += 'E';
  cam_s.write(Log);
  cam_s.write(CSV_STRING.c_str());
  cam_s.flush();
  Serial.println(CSV_STRING);         // For Debugging purposes
  CSV_STRING.clear();
  intr = 0;
  packet++;
}
if (chute_release == 1){
  if (first == 1){
  cam_s.write(take_pic);
  first = 0;
    }

  }
}