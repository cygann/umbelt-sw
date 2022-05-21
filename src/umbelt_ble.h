#include <bluefruit.h>
#include <string.h>
#include "Arduino.h"

struct BLEInterface {
    // OTA DFU service
    BLEDfu bledfu;

    // Uart over BLE service
    BLEUart bleuart;
};

void umbelt_ble_init();

void umbelt_ble_start_adv();

void umbelt_ble_read();
