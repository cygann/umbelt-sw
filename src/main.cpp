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
void haptics_test_multi();
void vibrate_single_motor(int loc, int dur);
void compass_update();
void compass_debug(float mag_x, float mag_y, float mag_z, float heading, int bin);
void set_battery_led();

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
    set_battery_led();
    // haptics_test_multi();
    // haptics_test();
    // compass_update();
}

void
compass_update() {
    // Read from Magnetometer
    sensors_event_t event; 
    lis3mdl.getEvent(&event);
    float x_off = 31.50;
    float y_off = -57.62;
    float z_off = 59.25;

    magnetic_x = event.magnetic.x - x_off;
    magnetic_y = event.magnetic.y - y_off;
    magnetic_z = event.magnetic.z - z_off;

    float Pi = 3.14159;

    // Calculate the angle of the vector x and z
    float heading = (atan2(magnetic_x, magnetic_z) * 180) / Pi;
    if (heading < 0) { 
        heading = 360 + heading;
    }

    // Resolve bin #
    // int bin = ((int) heading) / 30;
    int offset_heading = (((int) heading) + 15) % 360;
    int bin = offset_heading / 30;
    int bin2 = offset_heading / 15;

    // compass_debug(magnetic_x, magnetic_y, magnetic_x, heading, bin);
    Serial.println("");
    Serial.print(bin);
    Serial.print(" bin2: ");
    Serial.print(bin2);


    // if (i == bin && bin != prev_bin) {
    if (bin2 != prev_bin) {
        // vibrate_single_motor(i);
        // prev_bin = bin;
        //
        // If even, use true bin value
        if (bin2 % 2 == 0) {
            vibrate_single_motor(bin, 100);
            prev_bin = bin2;
        // Odd, do half power with two side motors 
        } else {

            int mot_1 = bin;
            int mot_2 = ceil(bin2 / 2.0);


            Serial.println("");
            Serial.print("half power with motors ");
            Serial.print(mot_1);
            Serial.print(" ");
            Serial.print(mot_2);

            digitalWrite(MOTOR_PINS[mot_1], HIGH);
            digitalWrite(MOTOR_PINS[mot_2], HIGH);
            delay(75);
            digitalWrite(MOTOR_PINS[mot_1], LOW);
            digitalWrite(MOTOR_PINS[mot_2], LOW);

            prev_bin = bin2;
        }
    }

}

void
vibrate_single_motor(int loc, int dur) {

    // If the motor lies on the back, then increase intensity
    if (loc >= 5 && loc <= 8) dur = 200;

    digitalWrite(MOTOR_PINS[loc], HIGH);
    delay(dur);
    digitalWrite(MOTOR_PINS[loc], LOW);
    delay(50);
    digitalWrite(MOTOR_PINS[loc], HIGH);
    delay(dur / 2);
    digitalWrite(MOTOR_PINS[loc], LOW);
}

void 
compass_debug(float mag_x, float mag_y, float mag_z, float heading, int bin) {
    Serial.println("");
    Serial.print("Magnetic: ");
    Serial.print(mag_x);
    Serial.print(" ");
    Serial.print(mag_y);
    Serial.print(" ");
    Serial.print(mag_z);
    Serial.print(" ");
    Serial.print("Compass Heading: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print("Bin: ");
    Serial.print(bin);
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

void
haptics_test_multi() {
    // Tests each haptic motor: vibrates each once, goes in a circle.
    for (int i = 0; i < 24; i++) {
        int mot_1 = (i / 2) % 12;
        int mot_2 = (int) ceil(i / 2.0) % 12;

        int dur = 150;
        Serial.print("Mot 1: ");
        Serial.print(mot_1);
        Serial.print(" mot 2: ");
        Serial.print(mot_2);
        Serial.println("");

        if (mot_1 != mot_2) dur = 2 * dur / 3;

        digitalWrite(MOTOR_PINS[mot_1], HIGH);
        digitalWrite(MOTOR_PINS[mot_2], HIGH);
        delay(dur);
        digitalWrite(MOTOR_PINS[mot_1], LOW);
        digitalWrite(MOTOR_PINS[mot_2], LOW);
        // delay(50);

        delay(500);
    }

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

