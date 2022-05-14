#include <bluefruit.h>
#include <string.h>
#include "Arduino.h"

struct BLEInterface {
    // OTA DFU service
    BLEDfu bledfu;

    // Uart over BLE service
    BLEUart bleuart;
};

void umbelt_ble_init(BLEInterface *ble);

void umbelt_ble_start_adv(BLEInterface *ble);
