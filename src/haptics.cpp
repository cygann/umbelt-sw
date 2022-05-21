#include <haptics.h>
// #include <NRF52TimerInterrupt.h>

void
init_haptics() {
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
 * @param motor_pin	    specified motor to vibrate
 * @param duration	    length time of vibration in ms
 * @param percent_motor percent of haptic vibration capacity to vibrate at,
 * 			                not to exceed 0.85
 * @param frequency     frequency of vibration (Hz)
 */
void actuate_hz(int motor_pin, int duration, double percent_motor, float frequency) {
  if (percent_motor > 0.85) {
    Serial.println("Motor percentage above 85%. Using 85%");
    percent_motor = 0.85;
  }

  if (frequency == 0) {
    delayMicroseconds(duration * 1000);
    return;
  }
  
  float half_period = 1 / (frequency * 2);
  int num_cycles = duration / (2 * half_period * 1000);
  digitalWrite(MOTOR_PINS[motor_pin] + EN_OFFSET, HIGH);  // enable

  for (int j = 0; j < num_cycles; j++) {
    analogWrite(MOTOR_PINS[motor_pin], 512 + percent_motor * 512);
    delayMicroseconds(half_period * 1000000);
    analogWrite(MOTOR_PINS[motor_pin], 512 - percent_motor * 512);
    delayMicroseconds(half_period * 1000000);
  }
  analogWrite(MOTOR_PINS[motor_pin], 512 + percent_motor * 512);
  delayMicroseconds(half_period / 2 * 1000000);
  analogWrite(MOTOR_PINS[motor_pin], 512);
  digitalWrite(MOTOR_PINS[motor_pin] + EN_OFFSET, LOW);  // disable
}

/* Vibrates specified motors for given intensity and duration.
 *
 * @param motor_pin	specified motor to vibrate
 * @param duration	length time of vibration in ms
 * @param percent_motor percent of haptic vibration capacity to vibrate at,
 * 			not to exceed 0.85
 */
void actuate_motor(int motor_pin, int duration, double percent_motor) {
  digitalWrite(MOTOR_PINS[motor_pin] + EN_OFFSET, HIGH);  // enable
  int half_period = 3;
  if (percent_motor > 0.85) {
    Serial.println("Motor percentage above 85%. Using 85%");
    percent_motor = 0.85;
  }
  percent_motor = 1;
  int numCycles = duration / (half_period * 2);
  for (int j = 0; j < numCycles; j++) {  // do J_MAX * TODAL_DELAY = 16 * 6 = 96ms of vibration
    analogWrite(MOTOR_PINS[motor_pin], 512 + percent_motor * 512);
    delayMicroseconds(half_period * 1000);
    analogWrite(MOTOR_PINS[motor_pin], 512 - percent_motor * 512);
    delayMicroseconds(half_period * 1000);
  }
  analogWrite(MOTOR_PINS[motor_pin], 512);
  digitalWrite(MOTOR_PINS[motor_pin] + EN_OFFSET, LOW);  // disable
}


void run_haptics(pulse* p, int num_pulses, int num_cycles/*, int motors[]*/) {
  for (int i = 0; i < num_cycles; i++) { // number of cycles to do total pattern
    for (int j = 0; j < num_pulses; j++) {
      actuate_hz(0, p[j].dur, 0.85, p[j].freq);
    }
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