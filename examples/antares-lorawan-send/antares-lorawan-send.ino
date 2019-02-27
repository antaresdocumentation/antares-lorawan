/*
    This is an example to send a "Hello world!" message
    to your Antares IoT Platform's LoRa device.

    The gateway uses AS923 frequency (Around 920-923MHz).

    sendPacket() is main function to send your data whenever your device is ready.
    Use antares.send(data) to send your data to Antares LoRaWAN Gateway.
    antares.send(data) accepts the datatype of
    - Arduino String
    - int
    - float
    - double

    For more information about the platform itself, please
    visit https://antares.id/id/index.html
*/

#include <AntaresLoRaWAN.h>

#define ACCESSKEY "b4e89ce2436b9d90:202c7b14b849c084"
#define DEVICEID "712ce6f2"

AntaresLoRaWAN antares;

int counter = 0;

void sendPacket() {
  String packet = "Hello world! " + String(counter);
  counter++;
  antares.send(packet);
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting"));

  antares.setPins(15, 4, 5);   // Set pins for NSS, DIO0, and DIO1
  antares.setTxInterval(5);    // Set the amount of interval time (in seconds) to transmit

  antares.init(ACCESSKEY, DEVICEID);

  // Set data rate and uplink TX power
  // Available data rate is DR_SF7 up to DR_SF12
  // Available power is 5dBm up to 20dBm
  antares.setDataRateTxPow(DR_SF7, 20);
}

void loop() {
  antares.runLoop();
}
