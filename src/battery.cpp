#include "Arduino.h"
#include "battery.h"

#define BATT_DEBUG 0
#define ANALOG_BITS 1024
#define IO_VOLTAGE 3.3
#define BATT_UPDATE_INTERVAL 1000  // update each second

const float voltage_max = 4.2;
const float voltage_low_threshold = 3.7;
const float voltage_drained_threshold = 3.6;

static batt_states batt_state = BATT_CHARGED;
static float batt_voltage = 0;
static unsigned long last_read_time = 0;

void check_battery() {
  if (millis() - last_read_time < BATT_UPDATE_INTERVAL) return;
  last_read_time = millis();

  pinMode(PIN_VBAT, INPUT);
  int analogCode = analogRead(PIN_VBAT);
  float voltage = (float)analogCode / ANALOG_BITS * IO_VOLTAGE;
  batt_voltage = voltage * 2;  // corrent for 1/2 voltage divider on board
  if (batt_voltage < voltage_drained_threshold)
    batt_state = BATT_DRAINED;
  else if (batt_voltage < voltage_low_threshold)
    batt_state = BATT_LOW;
  else
    batt_state = BATT_CHARGED;
#ifdef BATT_DEBUG
  Serial.print("Battery voltage: ");
  Serial.println(batt_voltage);
#endif
}

batt_states get_battery_state() {
  return batt_state;
}

float get_battery_voltage() {
  return batt_voltage;
}
