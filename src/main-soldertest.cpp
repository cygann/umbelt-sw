#include "Arduino.h"

#define M_RANGE_START MOTOR0  // first PWM pin
#define M_RANGE_END MOTOR12  // last PWM pin
#define PIN_OFFSET (MOTOR_EN0 - MOTOR0)  // offset between PWM and EN pin nums

void setup() {
  for (int i = M_RANGE_START; i <= M_RANGE_END; i++) {
    pinMode(i, OUTPUT);  // configure the PWM pin for output
    digitalWrite(i, HIGH);  // write the PWM pin high
    pinMode(i+PIN_OFFSET, OUTPUT);  // configure the EN pin for output
    digitalWrite(i+PIN_OFFSET, HIGH);  // write the EN pin high to enable
  }
}

void loop() {
  // repeatedly alternate each pin high and low every 5 seconds
  delay(5000);
  for (int i = M_RANGE_START; i <= M_RANGE_END; i++) {
    digitalWrite(i, LOW);
  }
  delay(5000);
  for (int i = M_RANGE_START; i <= M_RANGE_END; i++) {
    digitalWrite(i, HIGH);
  }
}
