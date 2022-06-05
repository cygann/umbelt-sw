#include <string.h>
#include "Arduino.h"

#define BLEUART_BUF_SIZE (32)

/* Initializes the Umbelt BLE module:
 * - Establishes event callbacks
 * - Adds UART service
 * - Begins advertising
 *
 * TODO: Pass in callbacks for when packet is found.
 */
void umbelt_ble_init(void);

/* Performs umbelt BLE tick.
 * Checks to see if bytes are available on the UART line. If so, parses data
 * packets and calls appropriate callback.
 */
void umbelt_ble_tick(void);

