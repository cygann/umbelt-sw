/*  Umbelt Compass module.
 *  Contains functions for reading from magnetometer and gyroscope sensors.
 *  Updates haptics based on these sensors.
 *  TODO: change this abstraction where magnetometer and gyro sensors are not
 *  stuck inside compass such that they could be used for other features.
 */

#include <Adafruit_LIS3MDL.h>
#include <Adafruit_LSM6DS33.h>
#include "Arduino.h"

const float Pi = 3.14159;

const int SAMPLING_N = 50; // Number of magnetometer measurements for denoising

const float X_OFFSET = 31.50; // Constants from magnetometer calibration
const float Y_OFFSET = -57.62;
const float Z_OFFSET = 59.25;

const float UPDATE_DUR = 200; // Time to for update haptics to persist, in ms

struct Compass {
    Adafruit_LIS3MDL lis3mdl;   // magnetometer
    Adafruit_LSM6DS33 lsm6ds33; // accelerometer, gyroscope

    // Current magnetometer readings for x, y, z
    int magnetic_x;
    int magnetic_y;
    int magnetic_z;

    int heading;    // Current heading, in degrees

    // Current gyroscope readings for x, y, z
    float gyro_x;
    float gyro_y;
    float gyro_z;

    unsigned long update_time; // Time in ms of last haptic update
    int update_heading; // Heading in deg of last haptic update
    bool motor_status; // True if motors are on, False if off
};

Compass init_compass();
void compass_update(Compass *compass);

void resolve_heading(Compass *compass);
void read_gyro(Compass *compass, bool verbose=false);

void compass_debug(Compass *compass, int bin);
