#include "AntaresLoRaWAN.h"
#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

lmic_pinmap lmic_pins;

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    String mydata = sendPacket();

    char dataBuf[50];
    int dataLen = mydata.length();

    mydata.toCharArray(dataBuf, dataLen + 1);

    Serial.println(mydata);
    Serial.println(dataBuf);

    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, (uint8_t*)dataBuf, dataLen, 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            Serial.println(F("Unknown event"));
            break;
    }
}

AntaresLoRaWAN::AntaresLoRaWAN() {
    // for(int i = 0; i < 16; i+=2) {
    //     unsigned long hexNwks;
    //     String nwks = (String)ACCESSKEY[i] + (String)ACCESSKEY[i+1];
    //     // Serial.println(nwks);
    //     char buf[nwks.length() + 1];
    //     nwks.toCharArray(buf, nwks.length() + 1);
    //     hexNwks = strtoul(buf, NULL, 16);
    //     // Serial.println(hexCode, HEX);
    //
    //     NWKSKEY[i/2] = (u1_t)hexNwks;
    //     // Serial.println(NWKSKEY[i/2], HEX);
    // }
    //
    // for(int j = 17; j < 33; j+=2) {
    //     unsigned long hexApps;
    //     String apps = (String)ACCESSKEY[j] + (String)ACCESSKEY[j+1];
    //     // Serial.println(apps);
    //     char buf[apps.length() + 1];
    //     apps.toCharArray(buf, apps.length() + 1);
    //     hexApps = strtoul(buf, NULL, 16);
    //     // Serial.println(hexCode, HEX);
    //
    //     APPSKEY[j/2] = (u1_t)hexApps;
    //     // Serial.println(APPSKEY[j/2], HEX);
    // }
    //
    // unsigned long hexAddr;
    // char bufAddr[DEVICEID.length() + 1];
    // DEVICEID.toCharArray(bufAddr, DEVICEID.length() + 1);
    // hexAddr = strtoul(bufAddr, NULL, 16);
    //
    // DEVADDR = (u4_t)hexAddr;
}

void AntaresLoRaWAN::setTxInterval(unsigned int intervalTime) {
    TX_INTERVAL = intervalTime;
}

void AntaresLoRaWAN::setDataRateTxPow(dr_t dr, s1_t txpow) {
    LMIC_setDrTxpow(dr, txpow);
}

void AntaresLoRaWAN::init(String ACCESSKEY, String DEVICEID) {
    static u1_t NWKSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   };
    // static u1_t NWKSKEY[16] = { 0xB4, 0xE8, 0x9C, 0xE2, 0x43, 0x6B, 0x9D, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // LoRaWAN AppSKey, application session key
    // This is the default Semtech key, which is used by the early prototype TTN
    // network.
    // static u1_t APPSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   };
    static u1_t APPSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x2C, 0x7B, 0x14, 0xB8, 0x49, 0xC0, 0x84 };

    // LoRaWAN end-device address (DevAddr)
    static u4_t DEVADDR = 0x0 ; // <-- Change this address for every node!
    // static u4_t DEVADDR = 0x712CE6F2 ; // <-- Change this address for every node!

    // Convert network key to unsigned int
    for(int i = 0; i < 16; i+=2) {
        unsigned long hexNwks;
        String nwks = (String)ACCESSKEY[i] + (String)ACCESSKEY[i+1];
        // Serial.println(nwks);
        char buf[nwks.length() + 1];
        nwks.toCharArray(buf, nwks.length() + 1);
        hexNwks = strtoul(buf, NULL, 16);
        // Serial.println(hexCode, HEX);

        NWKSKEY[i/2] = (u1_t)hexNwks;
        // Serial.println(NWKSKEY[i/2], HEX);
    }

    // Convert apps key to unsigned int
    for(int j = 17; j < 33; j+=2) {
        unsigned long hexApps;
        String apps = (String)ACCESSKEY[j] + (String)ACCESSKEY[j+1];
        // Serial.println(apps);
        char buf[apps.length() + 1];
        apps.toCharArray(buf, apps.length() + 1);
        hexApps = strtoul(buf, NULL, 16);
        // Serial.println(hexCode, HEX);

        APPSKEY[j/2] = (u1_t)hexApps;
        // Serial.println(APPSKEY[j/2], HEX);
    }

    // Convert device address to unsigned int
    unsigned long hexAddr;
    char bufAddr[DEVICEID.length() + 1];
    DEVICEID.toCharArray(bufAddr, DEVICEID.length() + 1);
    hexAddr = strtoul(bufAddr, NULL, 16);

    DEVADDR = (u4_t)hexAddr;

    // Print the Network key, Apps key, and Device Address
    for(int i = 0; i < 15; i++) {
        if(NWKSKEY[i] == 0) {
            Serial.print("00");
        }
        else {
            Serial.print(NWKSKEY[i], HEX);
        }
    }
    Serial.println();

    for(int i = 0; i < 15; i++) {
        if(APPSKEY[i] == 0) {
            Serial.print("00");
        }
        else {
            Serial.print(APPSKEY[i], HEX);
        }
    }
    Serial.println();
    Serial.println(DEVADDR, HEX);

    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    #if defined(CFG_eu868)
    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.
    // NA-US channels 0-71 are configured automatically
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.
    #elif defined(CFG_us915)
    // NA-US channels 0-71 are configured automatically
    // but only one group of 8 should (a subband) should be active
    // TTN recommends the second sub band, 1 in a zero based count.
    // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
    LMIC_selectSubBand(1);
#endif
    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    do_send(&sendjob);  // Start job
}

lmic_pinmap AntaresLoRaWAN::setPins(int nss, int dio0, int dio1) {
    lmic_pins = {
      .nss = nss,
      .rxtx = LMIC_UNUSED_PIN,
      .rst = LMIC_UNUSED_PIN,
      .dio = {dio0, dio1, LMIC_UNUSED_PIN},
    };
}

void AntaresLoRaWAN::startJob() {

}

void AntaresLoRaWAN::runLoop() {
    os_runloop_once();
}
