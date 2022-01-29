#include <haptics.h>

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

/* Vibrates specified motors for given intensity and duration.
 *
 * @param motor_pin	specified motor to vibrate
 * @param duration	length time of vibration in ms
 * @param percent_motor percent of haptic vibration capacity to vibrate at,
 * 			not to exceed 0.85
 */
void actuate_motor(int motor_pin, int duration, double percent_motor) {
  digitalWrite(MOTOR_PINS[motor_pin] + EN_OFFSET, HIGH);  // enable
  int frequency = 3;
  if (percent_motor > 0.85) {
    Serial.println("Motor percentage above 85%. Using 85%");
    percent_motor = 0.85;
  }
  int numCycles = duration / (frequency * 2);
  for (int j = 0; j < numCycles; j++) {  // do J_MAX * TODAL_DELAY = 16 * 6 = 96ms of vibration
    analogWrite(MOTOR_PINS[motor_pin], 512 + percent_motor * 512);
    delay(frequency);
    analogWrite(MOTOR_PINS[motor_pin], 512 - percent_motor * 512);
    delay(frequency);
  }
  digitalWrite(MOTOR_PINS[motor_pin] + EN_OFFSET, LOW);  // disable
} 

/* Repeats vibration mode for given duration
 * @param motor_pin	specified motor to vibrate
 * @param duration_on	length time of vibration in ms
 * @param percent_motor percent of haptic vibration capacity to vibrate at,
 * 			not to exceed 0.85
 * @param duration_off length of delay time between vibration cycle 
 */

void repeat_motor(int motor_pin, int repeat, int duration_on, int duration_off, double percent_motor) {
     for(int i = 0; i  < repeat; i++){
	 actuate_motor(motor_pin, duration_on, percent_motor);
	 delay(duration_off);
      }  
}


/* Test & debug functions */
void
haptics_test() {

  for (int i = 0; i < N_MOTORS; i++) {
    actuate_motor(i, 64, 0.85);
    delay(50);
  }
}

void
control_device(int duration, double percent_motor, int num_cycles) {
  for (int cycle = 0; cycle < numCycles; cycle++) {
    for (int i = 0; i < N_MOTORS; i++) {
      actuate_motor(i, duration, percent_motor);
      delay(50);
    }
  }
}

