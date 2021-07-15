#include "Arduino.h"
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS3MDL lis3mdl;   // magnetometer
int prev_bin;

const int MOTOR_PINS[12] = {12, A0, A1, A2, A3, A4, A5, 5, 6, 9, 10, 11};

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
}

void 
loop () {

    // Read from Magnetometer
    sensors_event_t event; 
    lis3mdl.getEvent(&event);
    float x_off = 31.50;
    float y_off = -57.62;
    float z_off = 59.25;

    magnetic_x = event.magnetic.x - x_off;
    magnetic_y = event.magnetic.y - y_off;
    magnetic_z = event.magnetic.z - z_off;

    Serial.println("");
    Serial.print("Magnetic: ");
    Serial.print(magnetic_x);
    Serial.print(" ");
    Serial.print(magnetic_y);
    Serial.print(" ");
    Serial.print(magnetic_z);

    float Pi = 3.14159;

    // Calculate the angle of the vector x and z
    float heading = (atan2(magnetic_x, magnetic_z) * 180) / Pi;
    if (heading < 0) { 
        heading = 360 + heading;
    }
    Serial.print(" ");
    Serial.print("Compass Heading: ");
    Serial.print(heading);
    Serial.print(" ");

    int bin = ((int) heading) / 30;

    Serial.print("Bin: ");
    Serial.print(bin);

    for (int i = 0; i < 12; i++) {

        if (i == bin) {
            digitalWrite(MOTOR_PINS[i], HIGH);
        } else {
            digitalWrite(MOTOR_PINS[i], LOW);
        }
    }
    

    // for (int i = 0; i < 12; i++) {
        // digitalWrite(MOTOR_PINS[i], HIGH);
        // delay(100);
        // digitalWrite(MOTOR_PINS[i], LOW);
        // delay(50);
        // digitalWrite(MOTOR_PINS[i], HIGH);
        // delay(50);
        // digitalWrite(MOTOR_PINS[i], LOW);
        // delay(50);
// 
        // delay(500);
    // }

}
