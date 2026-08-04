//======================================================================//

#include <SPI.h>
#include <LoRa.h>

//======================================================================//


#define PIN_LORA_COPI   23
#define PIN_LORA_CIPO   19
#define PIN_LORA_SCK    18
#define PIN_LORA_CS     5
#define PIN_LORA_RST    2
#define PIN_LORA_DIO0   4

#define LORA_FREQUENCY  433E6

//======================================================================//

void setup() {
  Serial.begin (115200);
  LoRa.setPins (PIN_LORA_CS, PIN_LORA_RST, PIN_LORA_DIO0);
  LoRa.setSPIFrequency (20000000);
  LoRa.begin(LORA_FREQUENCY);
  delay(1000);

  while (!LoRa.begin (LORA_FREQUENCY)) {
    Serial.println ("Starting LoRa failed");
    Serial.println (LoRa.begin (LORA_FREQUENCY));
    //ESP.restart();
  }
}

//======================================================================//

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {

    // read packet
    while (LoRa.available()) {
      Serial.print ((char) LoRa.read());
    }

    // print RSSI of packet
    Serial.print ("\n");
    //Serial.println (LoRa.packetRssi());
  }
}

//======================================================================//

