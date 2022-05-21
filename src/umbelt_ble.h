#include <bluefruit.h>
#include <string.h>
#include "Arduino.h"

#define BLEUART_BUF_SIZE (20)

struct BLEInterface {
    // OTA DFU service
    BLEDfu bledfu;

    // Uart over BLE service
    BLEUart bleuart;
};

void umbelt_ble_init();
void umbelt_ble_start_adv();

void umbelt_ble_read();

/* Reads from the incoming blueart datastream and puts data into packet buffer.
 *
 * @param blue_uart BLEUart interface.
 * @param timeout Read timeout (in characters to read)
 * TODO: Make timeout occur in ms.
 *
 * @return number of characters read from the BLEUart stream.
 */
uint8_t read_bleuart_packet(BLEUart *ble_uart, uint16_t timeout);


