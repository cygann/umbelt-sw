#include <Adafruit_LIS3MDL.h>
#include "Arduino.h"

const float Pi = 3.14159;

const float X_OFFSET = 31.50;
const float Y_OFFSET = -57.62;
const float Z_OFFSET = 59.25;

struct Compass {
    Adafruit_LIS3MDL lis3mdl;   // magnetometer
    int prev_bin;
    int magnetic_x;
    int magnetic_y;
    int magnetic_z;
    int heading;
    int last_update;
};

Compass init_compass();
void compass_update(Compass *compass);
void compass_update_continuous(Compass *compass);
void compass_simple(Compass *compass);

void resolve_heading(Compass *compass);
void compass_debug(Compass *compass, int bin);
