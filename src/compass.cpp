#include <Adafruit_LIS3MDL.h>
#include <compass.h>
#include <haptics.h>

Compass
compass_init() {

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

    // Calculate the angle of the vector x and z
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

    // compass_debug(magnetic_x, magnetic_y, magnetic_x, heading, bin);
    Serial.println("");
    Serial.print(bin);
    Serial.print(" bin2: ");
    Serial.print(bin2);


    // if (i == bin && bin != prev_bin) {
    if (bin2 != compass->prev_bin) {
        // vibrate_single_motor(i);
        // prev_bin = bin;
        //
        // If even, use true bin value
        if (bin2 % 2 == 0) {
            vibrate_single_motor(bin, 100);
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

            digitalWrite(MOTOR_PINS[mot_1], HIGH);
            digitalWrite(MOTOR_PINS[mot_2], HIGH);
            delay(75);
            digitalWrite(MOTOR_PINS[mot_1], LOW);
            digitalWrite(MOTOR_PINS[mot_2], LOW);

            compass->prev_bin = bin2;
        }
    }

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
