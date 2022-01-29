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
#define DEFAULT_TEST_CYCLES 2

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
  //haptics_test();
  if (Serial.available() > 0) {
    Serial.setTimeout(60000); // one minute

    Serial.println();
    Serial.println("Enter your percent_motor, followed by RET:");
    double percent_motor = Serial.parseInt() * 0.01; // turn to percent
    Serial.println("percent_motor: ");
    Serial.print(percent_motor);

    Serial.println();
    Serial.println("Enter your duration (ms), followed by RET:");
    int duration = Serial.parseInt();
    Serial.println("duration: ");
    Serial.print(duration);
    
    
    control_device(duration, percent_motor, DEFAULT_TEST_CYCLES);
    Serial.println("----COMPLETE!---");
  }
}

void
init_indicator_led() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}
