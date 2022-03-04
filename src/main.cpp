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
#include "CommandLine.h"
#include "MMC5633.h"

#define PIXEL_PIN 8
#define BAT_PIN 20
#define DEFAULT_TEST_CYCLES 2

Adafruit_NeoPixel pixels(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

BLEInterface ble;
Compass compass;

MMC5633 magneto;

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

    // while (!Serial) {}
    delay(1000); // For some reason, this is critical for Serial (?)
    Serial.println("This is Umbelt");
    // magneto = MMC5633();
    compass = Compass();
}

void
loop () {
    // haptics_test();
    compass.compass_update();
    // magneto.read();
}

void
init_indicator_led() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}
