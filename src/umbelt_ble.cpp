#include <bluefruit.h>

#include "umbelt_common.h"
#include "umbelt_ble.h"
#include "umbelt_haptics.h"


static uint8_t packet_buffer[BLEUART_BUF_SIZE];

// Struct that represents an instance of this module.
// connected : Whether umbelt is connected to ble central.
// bledfu : DFU service.
// bleuart : BLE Uart service.
typedef struct umbelt_ble {
  bool connected;
  BLEDfu bledfu;
  BLEUart bleuart;
} umbelt_ble;

// Instance of this class.
static umbelt_ble s_umbelt_ble;

/* Callback for event handler. */
void umbelt_ble_evt_handler(ble_evt_t* evt);

/* Sets up all advertising settings, starts advertising. */
void umbelt_ble_start_adv(void);

/* Reads from the incoming blueart datastream and puts data into packet buffer.
 *
 * @param blue_uart BLEUart interface.
 * @param timeout Read timeout (in characters to read)
 * TODO: Make timeout occur in ms.
 *
 * @return number of characters read from the BLEUart stream.
 */
uint8_t read_bleuart_packet(uint16_t read_timeout);

void umbelt_ble_init(void) {

  // Initialize instance of module
  rtt.println("Bluetooth init");

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName("Umbelt");
  rtt.println("Bluetooth init'd");

  // Set connect/disconnect callbacks.
  Bluefruit.setEventCallback(&umbelt_ble_evt_handler);

  // Start UART service
  s_umbelt_ble.bleuart.begin();
  rtt.println("BLE UART begin");

  // Start advertising.
  umbelt_ble_start_adv();
  rtt.println("Advertise begin");
}

void umbelt_ble_evt_handler(ble_evt_t* evt) {
  int evt_id = evt->header.evt_id;

  rtt.print("Event id is: ");
  rtt.println(evt_id);

  switch(evt_id) {

    case BLE_GAP_EVT_CONNECTED:
      s_umbelt_ble.connected = true;
      rtt.println("Connected to central.");
      break;

    case BLE_GAP_EVT_DISCONNECTED:
      s_umbelt_ble.connected = false;
      rtt.println("Disconnected from central.");
      break;

    default:
      break;
  }

}

void umbelt_ble_start_adv(void) {
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include the BLE UART (AKA 'NUS') 128-bit UUID
  Bluefruit.Advertising.addService(s_umbelt_ble.bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   *
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
  rtt.println("Advertise begin");
}

void umbelt_ble_tick(void) {
  uint8_t len = read_bleuart_packet(/*timeout=*/500);
  if (len == 0) return;
}

uint8_t read_bleuart_packet(uint16_t read_timeout) {
  uint16_t timeout = read_timeout;
  uint16_t idx = 0;

  memset(packet_buffer, 0, BLEUART_BUF_SIZE);

  int packet_size = -1;

  while (timeout--) {
    if (idx >= BLEUART_BUF_SIZE) break;
    if (idx == packet_size) break;

    while (s_umbelt_ble.bleuart.available()) {
      char c =  s_umbelt_ble.bleuart.read();
      if (c == '!') {
        idx = 0;
      }
      if (idx == 1) {
          packet_size = c;
          rtt.print(" Packet size: ");
          rtt.print(packet_size);
          rtt.print("  ");
      }

      rtt.print(idx);
      rtt.print(" ");
      rtt.println(c);

      packet_buffer[idx] = c;
      idx++;
      timeout = read_timeout;
    }

    if (timeout == 0) break;
    delay(1);
  }

  packet_buffer[idx] = 0;  // null term

  // no data or timeout
  if (!idx) return 0;

  // doesn't start with '!' packet beginning
  if (packet_buffer[0] != '!') return 0;

  // Return number of characters we've read.
  return idx;
}
