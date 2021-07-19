#include <Adafruit_LIS3MDL.h>
#include "Arduino.h"

const float Pi = 3.14159;

struct Compass {
    Adafruit_LIS3MDL lis3mdl;   // magnetometer
    int prev_bin;
    int magnetic_x;
    int magnetic_y;
    int magnetic_z;
    int heading;
};

Compass compass_init();
void compass_update(Compass *compass);
void compass_debug(float mag_x, float mag_y, float mag_z, float heading, int bin);
