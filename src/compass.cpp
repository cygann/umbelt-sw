#include <Adafruit_LIS3MDL.h>
#include <compass.h>
#include <haptics.h>

Compass
init_compass() {

    Compass c;
    c.prev_bin = -1;
    c.lis3mdl.begin_I2C(); // Init magnetometer

    return c;
}

void
compass_update(Compass *compass) {
    // Read from Magnetometer
    sensors_event_t event; 
    compass->lis3mdl.getEvent(&event);
    float x_off = 31.50;
    float y_off = -57.62;
    float z_off = 59.25;

    compass->magnetic_x = event.magnetic.x - x_off;
    compass->magnetic_y = event.magnetic.y - y_off;
    compass->magnetic_z = event.magnetic.z - z_off;

    // Calculate the angle of the vector x and z and resolve heading.
    float heading = (atan2(compass->magnetic_x, 
                compass->magnetic_z) * 180) / Pi;
    if (heading < 0) { 
        heading = 360 + heading;
    }
    compass->heading = heading;

    // Resolve bin #
    // int bin = ((int) heading) / 30;
    int offset_heading = (((int) heading) + 15) % 360;
    int bin = offset_heading / 30;
    int bin2 = offset_heading / 15;

    // compass_debug(&compass, bin);
    // Serial.println("");
    // Serial.print(bin);
    // Serial.print(" bin2: ");
    // Serial.print(bin2);

    int value = 255;

    if (bin2 != compass->prev_bin) {

        // If even, use true bin value
        if (bin2 % 2 == 0) {
            analogWrite(MOTOR_PINS[bin], value);
            delay(100);
            analogWrite(MOTOR_PINS[bin], 0);
            compass->prev_bin = bin2;
        // Odd, do half power with two side motors 
        } else {
            int mot_1 = bin;
            int mot_2 = ceil(bin2 / 2.0);

            Serial.println("");
            Serial.print("half power with motors ");
            Serial.print(mot_1);
            Serial.print(" ");
            Serial.print(mot_2);

            int new_val = value * 0.75;
            Serial.print(" New value: ");
            Serial.print(new_val);
            analogWrite(MOTOR_PINS[mot_1], new_val);
            analogWrite(MOTOR_PINS[mot_2], new_val);
            delay(100);
            analogWrite(MOTOR_PINS[mot_1], 0);
            analogWrite(MOTOR_PINS[mot_2], 0);

            compass->prev_bin = bin2;
        }
    }

}

void 
compass_debug(Compass *compass, int bin) {
    Serial.println("");
    Serial.print("Magnetic: ");
    Serial.print(compass->magnetic_x);
    Serial.print(" ");
    Serial.print(compass->magnetic_y);
    Serial.print(" ");
    Serial.print(compass->magnetic_z);
    Serial.print(" ");
    Serial.print("Compass Heading: ");
    Serial.print(compass->heading);
    Serial.print(" ");
    Serial.print("Bin: ");
    Serial.print(bin);
}
