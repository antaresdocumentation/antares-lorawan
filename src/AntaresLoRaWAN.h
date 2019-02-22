#ifndef AntaresLoRaWAN_H
#define AntaresLoRaWAN_H

#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

void do_send(osjob_t* j);

String sendPacket();

static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
static unsigned int TX_INTERVAL;

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
private:


};

#endif
