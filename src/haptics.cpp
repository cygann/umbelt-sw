#include <haptics.h>

void 
init_haptics() {
    // Initialize Motor Pins
    for (int i = 0; i < 12; i++) {
        pinMode(MOTOR_PINS[i], OUTPUT);
        digitalWrite(MOTOR_PINS[i], LOW);
    }
}

void
turn_on_motor_set(bool motors[], int value) {
    for (int i = 0; i < 12; i++) {
        // Turn on if true in the specified list
        if (motors[i]) {
            analogWrite(MOTOR_PINS[i], value);
        }
    }
}

void 
turn_off_motor_set(bool motors[]) {
    for (int i = 0; i < 12; i++) {
        // Turn off if true in the specified list
        if (motors[i]) {
            analogWrite(MOTOR_PINS[i], 0);
        }
    }
}

void
vibrate_single_motor(int loc, int dur) {

    // If the motor lies on the back, then increase intensity
    if (loc >= 4 && loc <= 7) dur = 200;

    digitalWrite(MOTOR_PINS[loc], HIGH);
    delay(dur);
    digitalWrite(MOTOR_PINS[loc], LOW);
    delay(50);
    digitalWrite(MOTOR_PINS[loc], HIGH);
    delay(dur / 2);
    digitalWrite(MOTOR_PINS[loc], LOW);
}

void
triple_buzz(bool motors[]) {

    int power = 255;
    turn_on_motor_set(motors, power);
    delay(100);
    turn_off_motor_set(motors);
    delay(90);

    turn_on_motor_set(motors, power);
    delay(100);
    turn_off_motor_set(motors);
    delay(90);

    turn_on_motor_set(motors, power);
    delay(100);
    turn_off_motor_set(motors);
    delay(90);

    turn_on_motor_set(motors, power);
    delay(100);
    turn_off_motor_set(motors);
    delay(90);

}

void 
triple_buzz_single_loc(int loc) {
    bool motors[12] = {false};
    motors[loc] = true;
    triple_buzz(motors);
}

void
surround_blink() {
    bool motors[12] = {false};
    motors[0] = true;
    motors[11] = true;
    motors[5] = true;
    motors[6] = true;

    // triple_buzz(motors);
    turn_on_motor_set(motors, 230);
    delay(150);
    turn_off_motor_set(motors);
    delay(150);
    turn_on_motor_set(motors, 230);
    delay(200);
    turn_off_motor_set(motors);

    for (int i = 0; i < N_MOTORS; i++) motors[i] = false;

    motors[2] = true;
    motors[3] = true;
    motors[9] = true;
    motors[8] = true;

    // triple_buzz(motors);
    turn_on_motor_set(motors, 230);
    delay(150);
    turn_off_motor_set(motors);
    delay(150);
    turn_on_motor_set(motors, 230);
    delay(150);
    turn_off_motor_set(motors);
    delay(50);
    turn_on_motor_set(motors, 230);
    delay(150);
    turn_off_motor_set(motors);
    delay(1000);

}

void
turn_off_all_motors() {
    for (int i = 0; i < 12; i++) {
        analogWrite(MOTOR_PINS[i], 0);
    }
}

/*  Determines if the passed motor would be considered to lie on the "back" of
 *  the belt. This is useful for determining which motors anatomically touch the
 *  user's back, where additional motor strength may wish to be applied.
 */
bool 
is_back_facing(int loc) {
    return (i >= 4 && i <= 7);
}


/* Test & debug functions */

void
analog_test() {

    int val_max = 40;
    // Test analog output at 5 different levels
    for (int i = val_max / 2; i <= val_max; i++) {
        int value = (i / (float)val_max) * 255;
        analogWrite(MOTOR_PINS[5], value);
        Serial.print("Index: ");
        Serial.println(i);
        delay(100);
        analogWrite(MOTOR_PINS[5], 0);
        delay(300);
    }

}

void
haptics_test() {
    // Tests each haptic motor: vibrates each once, goes in a circle.
    for (int i = 0; i < 12; i++) {
        Serial.println("");
        Serial.print("Motor: ");
        Serial.print(i);
        digitalWrite(MOTOR_PINS[i], HIGH);
        delay(400);
        digitalWrite(MOTOR_PINS[i], LOW);
        delay(50);
        digitalWrite(MOTOR_PINS[i], HIGH);
        delay(50);
        digitalWrite(MOTOR_PINS[i], LOW);
        delay(50);

        delay(500);
    }
}

void
haptics_test_multi() {
    // Tests each haptic motor: vibrates each once, goes in a circle.
    for (int i = 0; i < 24; i++) {
        int mot_1 = (i / 2) % 12;
        int mot_2 = (int) ceil(i / 2.0) % 12;

        int dur = 150;
        Serial.print("Mot 1: ");
        Serial.print(mot_1);
        Serial.print(" mot 2: ");
        Serial.print(mot_2);
        Serial.println("");

        if (mot_1 != mot_2) dur = 2 * dur / 3;

        digitalWrite(MOTOR_PINS[mot_1], HIGH);
        digitalWrite(MOTOR_PINS[mot_2], HIGH);
        delay(dur);
        digitalWrite(MOTOR_PINS[mot_1], LOW);
        digitalWrite(MOTOR_PINS[mot_2], LOW);
        // delay(50);

        delay(500);
    }
}
