#include "Arduino.h"
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 8 
#define BAT_PIN 20

Adafruit_LIS3MDL lis3mdl;   // magnetometer
Adafruit_NeoPixel pixels(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
int prev_bin;

const int MOTOR_PINS[12] = {12, A0, A1, A2, A3, A4, A5, 5, 6, 9, 10, 11};

void haptics_test();
void vibrate_single_motor(int loc);

float magnetic_x, magnetic_y, magnetic_z;
void 
setup () {

    // Initialize Motor Pins
    for (int i = 0; i < 12; i++) {
        pinMode(MOTOR_PINS[i], OUTPUT);
        digitalWrite(MOTOR_PINS[i], LOW);
    }

    Serial.begin(115200);
    Serial.println("Umbelt demo");

    lis3mdl.begin_I2C(); // Init magnetometer

    // Neopixel init
    pinMode(BAT_PIN, INPUT);
    pixels.begin();
    pixels.setBrightness(5);
    pixels.show();   // Send the updated pixel colors to the hardware.

}

void 
loop () {

    // Calculate battery voltage
    int voltage_raw = analogRead(BAT_PIN);
    Serial.print("Voltage raw: ");
    Serial.print(voltage_raw);
    Serial.println("");
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

    // Read from Magnetometer
    sensors_event_t event; 
    lis3mdl.getEvent(&event);
    float x_off = 31.50;
    float y_off = -57.62;
    float z_off = 59.25;

    magnetic_x = event.magnetic.x - x_off;
    magnetic_y = event.magnetic.y - y_off;
    magnetic_z = event.magnetic.z - z_off;

    // Serial.println("");
    // Serial.print("Magnetic: ");
    // Serial.print(magnetic_x);
    // Serial.print(" ");
    // Serial.print(magnetic_y);
    // Serial.print(" ");
    // Serial.print(magnetic_z);

    float Pi = 3.14159;

    // Calculate the angle of the vector x and z
    float heading = (atan2(magnetic_x, magnetic_z) * 180) / Pi;
    if (heading < 0) { 
        heading = 360 + heading;
    }
    // Serial.print(" ");
    // Serial.print("Compass Heading: ");
    // Serial.print(heading);
    // Serial.print(" ");

    int bin = ((int) heading) / 30;

    // Serial.print("Bin: ");
    // Serial.print(bin);

    for (int i = 0; i < 12; i++) {

        if (i == bin && bin != prev_bin) {
            vibrate_single_motor(i);
            prev_bin = bin;
        } else {
            digitalWrite(MOTOR_PINS[i], LOW);
        }
    }
}

void
vibrate_single_motor(int loc) {
    int dur = 100;

    // If the motor lies on the back, then increase intensity
    if (loc >= 5 && loc <= 8) dur = 200;

    digitalWrite(MOTOR_PINS[loc], HIGH);
    delay(dur);
    digitalWrite(MOTOR_PINS[loc], LOW);
}

void
haptics_test() {
    // Tests each haptic motor: vibrates each once, goes in a circle.
    for (int i = 0; i < 12; i++) {
        digitalWrite(MOTOR_PINS[i], HIGH);
        delay(100);
        digitalWrite(MOTOR_PINS[i], LOW);
        delay(50);
        digitalWrite(MOTOR_PINS[i], HIGH);
        delay(50);
        digitalWrite(MOTOR_PINS[i], LOW);
        delay(50);

        delay(500);
    }
}
