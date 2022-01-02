/*  Umbelt Haptics module.
 *  Contains functions for acuating and stopping haptic motors on the belt.
 */

#include <Adafruit_LIS3MDL.h>
#include "Arduino.h"

const int N_MOTORS = 13;
const int MOTOR_PINS[13] = {MOTOR0, MOTOR1, MOTOR2, MOTOR3, MOTOR4, MOTOR5,
                            MOTOR6, MOTOR7, MOTOR8, MOTOR9, MOTOR10, MOTOR11,
                            MOTOR12};
// Add EN_OFFSET to pin idx to get corresponding EN pin
const int EN_OFFSET = 13;
const int M_RANGE_START = 0;
const int M_RANGE_END = N_MOTORS;

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

/* --------- Haptic Utility functions ---------- */
bool is_back_facing(int loc);

/* --------- Test & Debug functions ---------- */
void haptics_test();
void haptics_test_multi();
