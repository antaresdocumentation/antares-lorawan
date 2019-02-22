/*
    This is an example to send a "Hello world!" message
    to your Antares IoT Platform's LoRa device.

    The gateway uses AS923 frequency (Around 923MHz).

    For more information about the platform itself, please
    visit https://antares.id/id/index.html
*/

#include <AntaresLoRaWAN.h>

#define ACCESSKEY "b4e89ce2436b9d90:202c7b14b849c084"
#define DEVICEID "712ce6f2"

AntaresLoRaWAN antares;

int counter = 0;

String sendPacket() {
  String packet = "Hello world! " + String(counter);
  counter++;
  return packet;
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting"));

  antares.setPins(15, 4, 5);   // Set pins for NSS, DIO0, and DIO1
  antares.setTxInterval(5);    // Set the amount of interval time (in seconds) to transmit

  antares.init(ACCESSKEY, DEVICEID);
  antares.setDataRateTxPow(DR_SF7, 20);   // Set data rate and uplink TX power
}

void loop() {
  antares.runLoop();
}
