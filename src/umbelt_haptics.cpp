#include "umbelt_haptics.h"

#define MICROSECONDS_PER_SECOND 1000000
#define ANALOG_SCALE 512
#define HALF_PERIOD 3
// #include <NRF52TimerInterrupt.h>

void umbelt_haptics_init(void) {
  // Initialize Motor Pins
  for (int i = 0; i < N_MOTORS; i++) {
    // Setup EN pin and set to low
    pinMode(MOTOR_PINS[i] + EN_OFFSET, OUTPUT);
    digitalWrite(MOTOR_PINS[i] + EN_OFFSET, LOW);
    pinMode(MOTOR_PINS[i], OUTPUT);
  }
}

/* Vibrates specified motors for given frequency.
 *
 * @param motor_idx	    specified motor to vibrate
 * @param duration	    length time of vibration in ms
 * @param percent_motor percent of haptic vibration capacity to vibrate at
 * @param frequency     frequency of vibration (Hz)
 */
void actuate_hz(int motor_idx, int duration, double percent_motor, float frequency) {
  if (percent_motor > 1 || percent_motor < 0) {
    Serial.println("Motor percentage out of bounds. Using 100%");
    percent_motor = 1;
  }

  percent_motor = percent_motor * 0.85;

  if (frequency == 0) {
    delayMicroseconds(duration * 1000);
    return;
  }

  float half_period = 1 / (frequency * 2);
  int num_cycles = duration / (2 * half_period * 1000);
  digitalWrite(MOTOR_PINS[motor_idx] + EN_OFFSET, HIGH);  // enable

  for (int j = 0; j < num_cycles; j++) {
    analogWrite(MOTOR_PINS[motor_idx], 512 + percent_motor * 512);
    delayMicroseconds(half_period * MICROSECONDS_PER_SECOND);
    analogWrite(MOTOR_PINS[motor_idx], 512 - percent_motor * 512);
    delayMicroseconds(half_period * 1000000);
  }
  analogWrite(MOTOR_PINS[motor_idx], 512 + percent_motor * 512);
  delayMicroseconds(half_period / 2 * 1000000);
  analogWrite(MOTOR_PINS[motor_idx], 512);
  digitalWrite(MOTOR_PINS[motor_idx] + EN_OFFSET, LOW);  // disable
}

/* Vibrates specified motors for given intensity and duration.
 *
 * @param motor_idx	specified motor to vibrate
 * @param duration	length time of vibration in ms
 * @param percent_motor percent of haptic vibration capacity to vibrate at,
 */
void actuate_motor(int motor_idx, int duration, double percent_motor) {
  digitalWrite(MOTOR_PINS[motor_idx] + EN_OFFSET, HIGH);  // enable
  if (percent_motor > 1 || percent_motor < 0) {
    Serial.println("Motor percentage out of bounds. Using 100%");
    percent_motor = 1;
  }
  percent_motor = percent_motor * 0.85;

  int numCycles = duration / (HALF_PERIOD * 2);
  for (int j = 0; j < numCycles; j++) {  // do J_MAX * TODAL_DELAY = 16 * 6 = 96ms of vibration
    analogWrite(MOTOR_PINS[motor_idx], ANALOG_SCALE + percent_motor * ANALOG_SCALE);
    delayMicroseconds(HALF_PERIOD * 1000);
    analogWrite(MOTOR_PINS[motor_idx], ANALOG_SCALE - percent_motor * ANALOG_SCALE);
    delayMicroseconds(HALF_PERIOD * 1000);
  }
  analogWrite(MOTOR_PINS[motor_idx], ANALOG_SCALE);
  digitalWrite(MOTOR_PINS[motor_idx] + EN_OFFSET, LOW);  // disable
}

/* Vibrates selected motor for given pulse pattern. A pulse pattern array
 * consists of the duration and frequency of each pulse.
 *
 * @param motor_idx	      specified motor to vibrate
 * @param p	              array of pulse pattern
 * @param num_pulses	    number of pulses in array
 * @param intensity        intensity of vibration
 */
void run_haptics(pulse* p, int num_pulses, int motor_idx, double intensity) {
  for (int j = 0; j < num_pulses; j++) {
    actuate_hz( motor_idx, p[j].dur, intensity, p[j].freq);
  }

}

/* Test & debug functions */
void
haptics_test() {
  for (int i = 0; i < N_MOTORS; i++) {
    actuate_motor(i, 120, 0.85);
    delay(1000);
  }
}
