#include <Adafruit_LIS3MDL.h>
#include "Arduino.h"

const int MOTOR_PINS[12] = {12, A0, A1, A2, A3, A4, A5, 5, 6, 9, 10, 11};

void init_haptics();
void vibrate_single_motor(int loc, int dur);
void triple_buzz(int loc);
void turn_off_all_motors();

/* --------- Test & Debug functions ---------- */
void analog_test();
void haptics_test();
void haptics_test_multi();
