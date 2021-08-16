#include <Adafruit_LIS3MDL.h>
#include <Adafruit_LSM6DS33.h>
#include <compass.h>
#include <haptics.h>

Compass
init_compass() {

    Compass c;
    c.prev_bin = -1;
    c.lis3mdl.begin_I2C(); // Init magnetometer
    c.lsm6ds33.begin_I2C();

    return c;
}

/*  Performs update to haptics system based on magnetometer and acceleration
 *  sensors. This function implements several key features of compass haptic
 *  feedback:
 *  - Obtains current heading
 *  - Ensures that haptic updates have UPDATE_DUR ms to occur before turning off
 *  the motors
 *  - Selectively sets threshold for new haptics depending on user movement,
 *  which employs the accelerometer. If the user is sitting still, a larger
 *  change in heading is needed to produce haptic feedback in order to
 *  counteract sensitivities in the magnetometer.
 *  - Uses analogWrite to create continuous transitions between motor
 *  boundaries. The strongest haptic feedback is provided around the motor most
 *  centered on the north heading. 
 */
void
compass_update(Compass *compass) {
    // TODO: Filter magnetometer data
    resolve_heading(compass);

    // Give UPDATE_DUR ms for the update haptics to complete
    unsigned long time = millis();
    if (time - compass->update_time >= UPDATE_DUR) {
        turn_off_all_motors();
        // if (compass->motor_status) Serial.println("Turning off all motors");
        compass->motor_status = false;
    }

    // Update every 10 degrees if moving, update every 20 degrees if not
    // moving. 
    int heading_diff = abs(compass->heading - compass->update_heading);
    bool moving = abs(compass->gyro_y) >= 0.20 ? true : false;
    if (moving && !(heading_diff >= 10)) {
        return;
    } else if (!moving && !(heading_diff >= 20)) {
        return;
    }

    // We update compass->motor_status = true here to indicate that the motors
    // are on, which is used for debugging this state machine. 
    compass->motor_status = true;
    unsigned long start = millis();

    // Update all motors
    int motor_values[N_MOTORS] = {0};
    for (int i = 0; i < 12; i++) {
        // Compute difference between the true north heading and the heading of
        // the motor in question.
        int bin_heading = i * 30;
        int diff = abs(compass->heading - bin_heading);

        int mot_val = 0;
        if (i >= 5 && i <= 8) { // Make the motors on the back stronger.
            mot_val = ((30 - diff) / 30.0) * 100 + 127;
        } else {
            mot_val = ((30 - diff) / 30.0) * 55 + 127;
        }
        // int mot_val = ((30 - diff) / 30.0) * 255;
        // Serial.println(mot_val);
        mot_val = mot_val >= 0 ? mot_val : 0;

        motor_values[i] = mot_val;
        analogWrite(MOTOR_PINS[i], motor_values[i]);
    }

    // The time in which we turn on the motors is recorded. This is checked
    // above to make sure that the motors have been on for UPDATE_DUR ms before
    // turning back off. 
    // We also record the heading at the time of this update to figure out
    // whether the heading has changed enough to update on the next reading.
    compass->update_time = millis();
    compass->update_heading = compass->heading;
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
compass_update_old(Compass *compass) {

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

    int mag_x = 0;
    int mag_y = 0;
    int mag_z = 0;

    // Read from Magnetometer: use the average of SAMPLING_N different
    // measurements to denoise.
    for (int i = 0; i < SAMPLING_N; i++) {
        sensors_event_t event; 
        compass->lis3mdl.getEvent(&event);
        mag_x += event.magnetic.x;
        mag_y += event.magnetic.y;
        mag_z += event.magnetic.z;
    }
    compass->magnetic_x = (mag_x / SAMPLING_N) - X_OFFSET;
    compass->magnetic_y = (mag_y / SAMPLING_N) - Y_OFFSET;
    compass->magnetic_z = (mag_z / SAMPLING_N) - Z_OFFSET;

    // Calculate the angle of the vector x and z and resolve heading.
    float heading = (atan2(compass->magnetic_x, 
                compass->magnetic_z) * 180) / Pi;
    if (heading < 0) { 
        heading = 360 + heading;
    }
    compass->heading = heading;
}

void
read_gyro(Compass *compass, bool verbose) {

    float gyro_x = 0;
    float gyro_y = 0;
    float gyro_z = 0;

    for (int i = 0; i < SAMPLING_N; i++) {
        sensors_event_t accel;
        sensors_event_t gyro;
        sensors_event_t temp;
        compass->lsm6ds33.getEvent(&accel, &gyro, &temp);

        gyro_x += gyro.acceleration.x;
        gyro_y += gyro.acceleration.y;
        gyro_z += gyro.acceleration.z;
    }

    compass->gyro_x = (gyro_x / SAMPLING_N);
    compass->gyro_y = (gyro_y / SAMPLING_N);
    compass->gyro_z = (gyro_z / SAMPLING_N);

    if (verbose) {
        Serial.println("");
        Serial.print("Gyro data: x: ");
        Serial.print(compass->gyro_x);
        Serial.print(" y: ");
        Serial.print(compass->gyro_y);
        Serial.print(" z: ");
        Serial.print(compass->gyro_z);
    }

    // float move_mag = sqrt(compass->gyro_x * compass->gyro_x + compass->gyro_z * compass->gyro_z);
// 
    // Serial.print(" Magnitude: ");
    // Serial.print(move_mag);

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
