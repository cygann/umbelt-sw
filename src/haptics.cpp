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

/* Test & debug functions */
void
haptics_test() {
    for (int i = 0; i < N_MOTORS; i++) {
        digitalWrite(MOTOR_PINS[i] + EN_OFFSET, HIGH);  // enable
        for (int j = 0; j < 64; j++){  // do J_MAX * TODAL_DELAY = 16 * 6 = 96ms of vibe
            analogWrite(MOTOR_PINS[i], 512 + 0.85 * 512);
            delay(3);
            analogWrite(MOTOR_PINS[i], 512 - 0.85 * 512);
            delay(3);
        }
        digitalWrite(MOTOR_PINS[i] + EN_OFFSET, LOW);  // disable
        delay(50);
    }
}
