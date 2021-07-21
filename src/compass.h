#include <Adafruit_LIS3MDL.h>
#include "Arduino.h"

const float Pi = 3.14159;

const int SAMPLING_N = 9; // Number of magnetometer measurements for denoising

const float X_OFFSET = 31.50; // Constants from magnetometer calibration
const float Y_OFFSET = -57.62;
const float Z_OFFSET = 59.25;

const float update_dur = 200; // Time to for update haptics to persist, in ms

struct Compass {
    Adafruit_LIS3MDL lis3mdl;   // magnetometer
    int prev_bin;
    int magnetic_x;
    int magnetic_y;
    int magnetic_z;
    int heading;
    unsigned long update_time; // Time in ms of last haptic update
    int update_heading; // Heading in deg of last haptic update
    bool motor_status; // True if motors are on, False if off
};

Compass init_compass();
void compass_update(Compass *compass);
void compass_update_continuous(Compass *compass);
void compass_simple(Compass *compass);

void resolve_heading(Compass *compass);
void compass_debug(Compass *compass, int bin);
