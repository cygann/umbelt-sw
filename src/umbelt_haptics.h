/*  Umbelt Haptics module.
 *  Contains functions for acuating and stopping haptic motors on the belt.
 */

#include <Arduino.h>

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

void umbelt_haptics_init(void);

/* --------- Test & Debug functions ---------- */
void haptics_test();
void control_device(int duration, double percent_motor, int num_cycles);
void actuate_motor(int motor_idx, int duration, double percent_motor);
void run_haptics(pulse* p, int num_pulses, int motor_idx, double intensity);
void actuate_hz(int motor_pin, int duration, double percent_motor, float frequency);
