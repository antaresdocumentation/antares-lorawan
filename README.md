# Antares LoRaWAN

<img src="http://i68.tinypic.com/2uptonn.jpg" width="300">  

This is a library which consists of high-level API for connecting your LoRa device to Antares IoT Platform LoRaWAN Gateway.   

This library works under the **Arduino** platform and has been tested on these boards:

* ESP8266 (WeMos D1 Mini)
* ESP32 (ESP32 Dev Board)
* STM32F103 (Bluepill)

and these LoRa modems:

* RFM95

# Pin Connections

These are the pin connections which needs to be provided in order to make this library work.

| Modem | Device |
|-------|--------|
| SCK   | SCK    |
| MISO  | MISO   |
| MOSI  | MOSI   |
| NSS   | (Custom GPIO Pin) |
| DIO0  | (Custom GPIO Pin) |
| DIO1  | (Custom GPIO Pin) |

The pin configuration can be changed with the following method of class `AntaresLoRaWAN` :
```
AntaresLoRaWAN::setPins(int nssPin, int dio0Pin, int dio1Pin);
```
The class needs to be insantiated first before using the method. Please refer to the example for the pin connections.  

These pinmaps have been tested on these boards to be working:
```
ESP8266 : SCK=14(D5) MISO=12(D6) MOSI=13(D7) NSS=15(D8) DIO0=4(D1) DIO1=5(D2)
ESP32   : SCK=18 MISO=19 MOSI=23 NSS=2 DIO0=22 DIO1=21
ESP32   : SCK=A5 MISO=A6 MOSI=A7 NSS=A4 DIO0=A3 DIO1=A0
```

# Installation

* Install Arduino IDE
* Download this library in .zip format and put it into your Arduino IDE's library catalog (**Sketch > Include Library > Add .ZIP Library...**)

This library is an adaptation of [Arduino LMIC AS923](https://github.com/orenonline/arduino-lmic-as923).  

<img src="https://upload.wikimedia.org/wikipedia/id/thumb/c/c4/Telkom_Indonesia_2013.svg/1200px-Telkom_Indonesia_2013.svg.png" width="200">  

**Powered by Telkom Indonesia**  
