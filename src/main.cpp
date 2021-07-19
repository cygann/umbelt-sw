#include "Arduino.h"
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>

/* Umbelt library code */
#include "haptics.h"
#include "compass.h"

#define PIXEL_PIN 8 
#define BAT_PIN 20

Adafruit_NeoPixel pixels(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

Compass compass;

void set_battery_led();

// float magnetic_x, magnetic_y, magnetic_z;
void 
setup () {

    // Initialize Motor Pins
    for (int i = 0; i < 12; i++) {
        pinMode(MOTOR_PINS[i], OUTPUT);
        digitalWrite(MOTOR_PINS[i], LOW);
    }

    Serial.begin(115200);
    Serial.println("Umbelt demo");

    // lis3mdl.begin_I2C(); // Init magnetometer
    compass = compass_init();


    // Neopixel init
    pinMode(BAT_PIN, INPUT);
    pixels.begin();
    pixels.setBrightness(5);
    pixels.show();   // Send the updated pixel colors to the hardware.

}

void 
loop () {
    set_battery_led();
    // analog_test();
    // haptics_test_multi();
    // haptics_test();
    compass_update(&compass);
}

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

