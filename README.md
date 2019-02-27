# Antares LoRaWAN

<img src="http://i68.tinypic.com/2uptonn.jpg" width="300">  

This is a library which consists of high-level API for connecting your LoRa device to Antares IoT Platform LoRaWAN Gateway.   

This library works under the **Arduino** platform and has been tested on these boards:

* ESP8266

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
setPins(int nssPin, int dio0Pin, int dio1Pin);
```
The class needs to be insantiated first before using the method. Please refer to the example for the pin connections.

# Installation

* Install Arduino IDE
* Download this library in .zip format and put it into your Arduino IDE's library catalog (**Sketch > Include Library > Add .ZIP Library...**)

This library is an adaptation of [Arduino LMIC AS923](https://github.com/orenonline/arduino-lmic-as923).  

<img src="https://upload.wikimedia.org/wikipedia/id/thumb/c/c4/Telkom_Indonesia_2013.svg/1200px-Telkom_Indonesia_2013.svg.png" width="200">  

**Powered by Telkom Indonesia**  
