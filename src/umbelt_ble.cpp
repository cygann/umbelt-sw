#include <bluefruit.h>
#include <umbelt_common.h>

#include <umbelt_ble.h>
#include <umbelt_haptics.h>

static BLEInterface s_ble;

static uint8_t packet_buffer[BLEUART_BUF_SIZE];

void umbelt_ble_init() {

  // while (!Serial) delay(10);
  rtt.println("Bluetooth init");

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  rtt.println("Bluetooth init'd");

  s_ble.bleuart.begin();
  rtt.println("BLE UART begin");

  Bluefruit.setName("Umbelt");

  umbelt_ble_start_adv();
  rtt.println("Advertise begin");
}

void umbelt_ble_start_adv() {
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include the BLE UART (AKA 'NUS') 128-bit UUID
  Bluefruit.Advertising.addService(s_ble.bleuart);

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

void umbelt_ble_read() {
  uint8_t len = read_bleuart_packet(&(s_ble.bleuart), 500);
  if (len == 0) return;
}

uint8_t read_bleuart_packet(BLEUart *ble_uart, uint16_t read_timeout) {
  uint16_t timeout = read_timeout;
  uint16_t idx = 0;

  memset(packet_buffer, 0, BLEUART_BUF_SIZE);

  int packet_size = -1;

  while (timeout--) {
    if (idx >= BLEUART_BUF_SIZE) break;
    if (idx == packet_size) break;

    while (ble_uart->available()) {
      char c =  ble_uart->read();
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
