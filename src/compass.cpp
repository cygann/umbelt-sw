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
compass_update_continuous(Compass *compass) {
    // TODO: Add non-constant vibration, and hysteresis
    resolve_heading(compass);

    int offset_heading = (((int) compass->heading) + 15) % 360;

    int motor_values[12] = {0};

    for (int i = 0; i < 12; i++) {
        int bin_heading = i * 30;
        int diff = abs(compass->heading - bin_heading);

        int mot_val = ((30 - diff) / 30.0) * 127 + 127;
        // int mot_val = ((30 - diff) / 30.0) * 255;
        Serial.println(mot_val);
        mot_val = mot_val >= 0 ? mot_val : 0;

        motor_values[i] = mot_val;
        analogWrite(MOTOR_PINS[i], motor_values[i]);
    }
}

void
compass_simple(Compass *compass) {
    resolve_heading(compass);
    int offset_heading = (((int) compass->heading) + 15) % 360;
    int bin = offset_heading / 30;

    for (int i = 0; i < 12; i++) {
        if (i == bin && bin != compass->prev_bin) {
            vibrate_single_motor(i, 100);
            compass->prev_bin = bin;
        } else {
            digitalWrite(MOTOR_PINS[i], LOW);
        }
    }
} 

void
compass_update(Compass *compass) {

    resolve_heading(compass);

    // Resolve bin #
    // int bin = ((int) heading) / 30;
    int offset_heading = (((int) compass->heading) + 15) % 360;
    int bin = offset_heading / 30;
    int bin2 = offset_heading / 15;

    // compass_debug(&compass, bin);
    // Serial.println("");
    // Serial.print(bin);
    // Serial.print(" bin2: ");
    // Serial.print(bin2);

    int value = 255;
    int dur = 75;

    if (bin2 != compass->prev_bin) {
        // If the motor lies on the back, then increase intensity
        if (bin >= 5 && bin <= 8) value = 255;

        // If even, use true bin value
        if (bin2 % 2 == 0) {
            analogWrite(MOTOR_PINS[bin], value);
            delay(dur);
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
            delay(dur);
            analogWrite(MOTOR_PINS[mot_1], 0);
            analogWrite(MOTOR_PINS[mot_2], 0);

            compass->prev_bin = bin2;
        }
    }

}

// Reads from Magnetometer to get x, y, and z magentic fields, then resolves the
// heading (in degrees). Updates the compass object to have this heading.
void
resolve_heading(Compass *compass) {
    // Read from Magnetometer
    sensors_event_t event; 
    compass->lis3mdl.getEvent(&event);

    compass->magnetic_x = event.magnetic.x - X_OFFSET;
    compass->magnetic_y = event.magnetic.y - Y_OFFSET;
    compass->magnetic_z = event.magnetic.z - Z_OFFSET;

    // Calculate the angle of the vector x and z and resolve heading.
    float heading = (atan2(compass->magnetic_x, 
                compass->magnetic_z) * 180) / Pi;
    if (heading < 0) { 
        heading = 360 + heading;
    }
    compass->heading = heading;
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
