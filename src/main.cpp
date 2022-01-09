#include "Arduino.h"
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>
#include <bluefruit.h>

/* Umbelt library code */
#include "bluetooth.h"
#include "haptics.h"
#include "compass.h"
#include "clock.h"

#define PIXEL_PIN 8
#define BAT_PIN 20

Adafruit_NeoPixel pixels(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

BLEInterface ble;
Compass compass;

extern uint8_t packetbuffer[];

// void set_battery_led();
void init_indicator_led();

void
setup () {

    Serial.begin(115200);
    Serial.println("Umbelt demo");

    // Init umbelt modules
    init_haptics();
    init_indicator_led();

    haptics_test();
    // compass = init_compass();

    // Neopixel init
    // pinMode(BAT_PIN, INPUT);
    // pixels.begin();
    // pixels.setBrightness(5);
    // pixels.show();   // Send the updated pixel colors to the hardware.

    // Init bluetooth modules
    // init_bluetooth(&ble);
}

void
loop () {
    // haptics_test();
    // set_battery_led();
    // analog_test();
    // haptics_test_multi();
    // haptics_test();

    // compass_update(&compass);
    // surround_blink();
    // read_gyro(&compass);

    // bluetooth_read(&ble);
}

void
init_indicator_led() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

/* Adafruit feathersense neopixel
void
set_battery_led() {
    // Calculate battery voltage
    int voltage_raw = analogRead(BAT_PIN);
    // Serial.print("Voltage raw: ");
    // Serial.print(voltage_raw);
    // Serial.println("");
    float voltage = (voltage_raw / 1023.0) * 2 * 3.3;

    // Set Pixel color based on battery charge
    if (voltage <= 3.4) {
        pixels.setPixelColor(0, 120, 0, 0);
    } else if (voltage > 3.4 && voltage <= 3.8) {
        pixels.setPixelColor(0, 30, 120, 0);
    } else {
        pixels.setPixelColor(0, 0, 180, 0);
    }
    pixels.show();   // Send the updated pixel colors to the hardware.
}
*/
