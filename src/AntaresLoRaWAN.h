#ifndef AntaresLoRaWAN_H
#define AntaresLoRaWAN_H

#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <RH_RF95.h>

void do_send(osjob_t* j);

void sendPacket();

// Sleep mode variables
static bool sleepStatus;
static unsigned int sleepInterval;

static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
static unsigned int TX_INTERVAL;

static String dataToSend;

void onEvent (ev_t ev);

class AntaresLoRaWAN {
public:
    AntaresLoRaWAN();
    lmic_pinmap setPins(int nss, int dio0, int dio1);
    void setTxInterval(unsigned int intervalTime);
    void setDataRateTxPow(dr_t dr, s1_t txpow);
    u1_t getNwksKey();
    u1_t getAppsKey();
    void startJob();
    void runLoop();
    void init(String ACCESSKEY, String DEVICEID);

    // Set sleep mode
    void setSleep(bool sleepSt);
    void setSleep(bool sleepSt, unsigned int sleepInt);

    // Add data in JSON format
    void add(String data);
    void add(int data);
    void add(float data);
    void add(double data);
    void send();

    // Send unformatted data
    void send(String data);
    void send(int data);
    void send(float data);
    void send(double data);
private:
    uint8_t _nssPin;
    uint8_t _dio0Pin;
    uint8_t _dio1Pin;
};

#endif
