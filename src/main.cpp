#include "Arduino.h"

const int MOTOR_PINS[12] = {12, A0, A1, A2, A3, A4, A5, 5, 6, 9, 10, 11};

void 
setup () {
    for (int i = 0; i < 12; i++) {
        pinMode(MOTOR_PINS[i], OUTPUT);
        digitalWrite(MOTOR_PINS[i], LOW);
    }

}

void 
loop () {
    for (int i = 0; i < 12; i++) {
        digitalWrite(MOTOR_PINS[i], HIGH);
        delay(100);
        digitalWrite(MOTOR_PINS[i], LOW);
        delay(50);
        digitalWrite(MOTOR_PINS[i], HIGH);
        delay(50);
        digitalWrite(MOTOR_PINS[i], LOW);
        delay(50);

        delay(500);
    }
}
