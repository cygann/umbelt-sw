#include <umbelt_compass.h>
#include <umbelt_haptics.h>
#include <mmc5633.h>

/* Initializes the compass struct object by begining I2C on the magnetometer and
 * gyroscope devices.
 */
Compass::Compass() {
    magneto = MMC5633();
    this->heading = 0;
    this->prev_heading_update = 0;
    this->update_time = 0;
    this->prev_motor_update = 0;
}

/* Determines if the current heading is greater than thresh degrees from the
 * last heading when a haptic vibration was produced.
 */
bool
Compass::heading_change_greater_than(float thresh) {
    float first_angle = abs(this->heading - this->prev_heading_update);
    float second_angle = 360 - first_angle;
    float smaller = min(first_angle, second_angle);
    // Serial.print("heading: ");
    // Serial.print(this->heading);
    // Serial.print(", prev_heading: ");
    // Serial.print(this->prev_heading_update);
    // Serial.print(", change: ");
    // Serial.println(smaller);
    return smaller > thresh;
}

/*  Performs update to haptics system based on magnetometer and gyroscope
 *  sensors. This function implements several key features of compass haptic
 *  feedback:
 *  - Obtains current heading
 *  - Ensures that haptic updates have UPDATE_DUR ms to occur before turning off
 *  the motors
 *  - Selectively sets threshold for new haptics depending on user movement,
 *  which employs the gyroscope. If the user is sitting still, a larger
 *  change in heading is needed to produce haptic feedback in order to
 *  counteract sensitivities in the magnetometer.
 *  - Uses analogWrite to create continuous transitions between motor
 *  boundaries. The strongest haptic feedback is provided around the motor most
 *  centered on the north heading.
 */
void
Compass::compass_update() {
    if (millis() > this->update_time + 1000) {
        resolve_heading(); // Every second vibrate the motor
        if (!heading_change_greater_than(15)) return;
        int bin = 13 - (this->heading * N_MOTORS) / 360;
        // Serial.print("Bin: ");
        // Serial.println(bin);
        actuate_motor(bin, 80, 0.85);
        this->prev_motor_update = bin;
        this->update_time = millis();
        this->prev_heading_update = this->heading;
    }

}

// Reads from Magnetometer to get x, y, and z magentic fields, then resolves the
// heading (in degrees). Updates the compass object to have this heading.
void
Compass::resolve_heading() {

    int start_time = millis();

    int mag_x = 0;
    int mag_y = 0;
    int mag_z = 0;

    // Read from Magnetometer: use the average of SAMPLING_N different
    // measurements to denoise.
    for (int i = 0; i < SAMPLING_N; i++) {
        // sensors_event_t event;
        mag_reading reading;
        bool success = magneto.read(&reading);
        mag_x += reading.mag_x;
        mag_y += reading.mag_y;
        mag_z += reading.mag_z;

        // if (!success) {
            // Serial.println("Error with read");
        // }
    }
    this->magnetic_x = (mag_x / SAMPLING_N) - X_OFFSET;
    this->magnetic_y = (mag_y / SAMPLING_N) - Y_OFFSET;
    this->magnetic_z = (mag_z / SAMPLING_N) - Z_OFFSET;

    // Calculate the angle of the vector x and z and resolve heading.
    float heading = (atan2(magnetic_x, magnetic_z) * 180) / Pi;
    if (heading < 0) {
        heading = 360 + heading;
    }
    this->heading = heading;
    // Serial.print("Compass heading: ");
    // Serial.println(this->heading);

    // Serial.print("heading update elasped time: ");
    // Serial.println(millis() - start_time);
}

/*  Reads from gyroscope. Updates gyro_x, gyro_y, and gyro_z fields of the
 *  compass object received as argument.
 *  Includes simple denoising through average of SAMPLING_N samples.
 */
void
read_gyro(bool verbose) {

    /*
    float gyro_x = 0;
    float gyro_y = 0;
    float gyro_z = 0;

    // Read SAMPLING_N samples, average over them.
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
    */
}

void
Compass::compass_debug() {
    // Serial.println("");
    // Serial.print("Magnetic: ");
    // Serial.print(this->magnetic_x);
    // Serial.print(" ");
    // Serial.print(this->magnetic_y);
    // Serial.print(" ");
    // Serial.print(this->magnetic_z);
    // Serial.print(" ");
    // Serial.print("Compass Heading: ");
    // Serial.print(this->heading);
    // Serial.print(" ");
    // Serial.print("Bin: ");
    // Serial.print(bin);
}
