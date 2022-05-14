#include <bluefruit.h>

#include <umbelt_ble.h>
#include <haptics.h>

void
umbelt_ble_init(BLEInterface *ble) {

  // while (!Serial) delay(10);
  Serial.println("Starting up");

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values

  // Configure and start the BLE Uart service
  // ble->bleuart.begin();
  // Serial.println("bleuart begin");

  // Set up and start advertising
  // startAdv(ble);
  // Serial.println("advertise begin");
//
  // Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  // Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  // Serial.println();
}

void umbelt_ble_start_adv(BLEInterface *ble)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include the BLE UART (AKA 'NUS') 128-bit UUID
  Bluefruit.Advertising.addService(ble->bleuart);

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
}
