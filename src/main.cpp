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
}

void
loop () {
    haptics_test();

}

void
init_indicator_led() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}
