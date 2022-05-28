#include "Arduino.h"
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>

/* Umbelt library code */
#include "umbelt_ble.h"
#include "umbelt_common.h"
#include "umbelt_haptics.h"
#include "umbelt_haptic_patterns.h"
#include "umbelt_compass.h"
#include "umbelt_clock.h"
#include "CommandLine.h"
#include "MMC5633.h"

#define PIXEL_PIN 8
#define BAT_PIN 20
#define DEFAULT_TEST_CYCLES 2

#define UMBELT_COMPASS_ENABLE true

Adafruit_NeoPixel pixels(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

BLEInterface ble;

extern uint8_t packetbuffer[];

// void set_battery_led();
void init_indicator_led();

void
setup () {
    rtt.println(" ======== Umbelt =====|=[] ");

    // Init umbelt modules
    init_haptics();
    init_indicator_led();
    umbelt_compass_init(UMBELT_COMPASS_ENABLE);
}

void
loop () {
    run_haptics(bach, /*num_pulses=*/32, /*motor_idx=*/0, /*intensity=*/0.85);

    // compass.compass_update()s;
    // magneto.read();
    // delay(100);
}

void
init_indicator_led() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}
