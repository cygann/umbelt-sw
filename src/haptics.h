#include <Adafruit_LIS3MDL.h>
#include "Arduino.h"

const int N_MOTORS = 12;
const int MOTOR_PINS[12] = {12, A0, A1, A2, A3, A4, A5, 5, 6, 9, 10, 11};

void init_haptics();

/* --------- Primary Haptic Functions ---------- */
void turn_on_motor_set(bool motors[], int value);
void turn_off_motor_set(bool motors[]);
void vibrate_single_motor(int loc, int dur);
void turn_off_all_motors();

/* --------- Specialty Haptics ---------- */
void triple_buzz(bool motors[]);
void triple_buzz_single_loc(int loc);
void surround_blink();

/* Haptic Utility functions */
bool is_back_facing(int loc);

/* --------- Test & Debug functions ---------- */
void analog_test();
void haptics_test();
void haptics_test_multi();
