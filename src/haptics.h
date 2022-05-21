/*  Umbelt Haptics module.
 *  Contains functions for acuating and stopping haptic motors on the belt.
 */

#include <Adafruit_LIS3MDL.h>
#include "Arduino.h"
#define RESONANT_FREQ 170 // in Hz

const int N_MOTORS = 13;
const int MOTOR_PINS[13] = {MOTOR0, MOTOR1, MOTOR2, MOTOR3, MOTOR4, MOTOR5,
                            MOTOR6, MOTOR7, MOTOR8, MOTOR9, MOTOR10, MOTOR11,
                            MOTOR12};
// Add EN_OFFSET to pin idx to get corresponding EN pin
const int EN_OFFSET = 13;
const int M_RANGE_START = 0;
const int M_RANGE_END = N_MOTORS;

typedef struct pulse {
  float freq; // vibration frequency
  float dur;  // duration in ms
} pulse;

typedef struct repeat_pulse {
  pulse* p; // pointer to pulse
  int repeat_num;
} repeat_pulse;

void init_haptics();

/* --------- Test & Debug functions ---------- */
void haptics_test();
void control_device(int duration, double percent_motor, int num_cycles);
void actuate_motor(int motor_pin, int duration, double percent_motor);
void run_haptics(pulse* p, int num_pulses, int num_cycles);
void actuate_hz(int motor_pin, int duration, double percent_motor, float frequency);

/* --------- Haptic Sample Library ---------- */
static float noteFrequenciesExt[11] {98, 110, 123, 131, 147, 165, 175, 196, 220, 247, 261}; // G2 --> C4

static pulse jackhammer[2] = {
  {150, 100},
  {0, 100}
};

static pulse bach[32] = {
  {noteFrequenciesExt[0], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[8], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[0], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[8], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[4], 100},
  {noteFrequenciesExt[1], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[1], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[9], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[5], 100},
  {noteFrequenciesExt[10], 100},
  {noteFrequenciesExt[5], 100}
};
static pulse maryHadALittleLamb [29] = {
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[3], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {0, 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[4], 1000},
  {0, 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[7], 1000},
  {noteFrequenciesExt[7], 1000},
  {0, 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[3], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[5], 1000},
  {noteFrequenciesExt[4], 1000},
  {noteFrequenciesExt[3], 1000},
};