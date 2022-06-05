#include <Arduino.h>

#include "umbelt_battery.h"

#define BATT_DEBUG 0
#define ANALOG_BITS 1024
#define IO_VOLTAGE 3.3
#define BATT_UPDATE_INTERVAL 1000  // update each second

#define VOLTAGE_MAX 4.2
#define VOLTAGE_LOW_THRESHOLD 3.7
#define VOLTAGE_DRAINED_THRESHOLD 3.6


typedef struct umbelt_batt {
  bool batt_initialized;  // is the battery module initialized?
  long long last_read_time;  // last time state was updated
  batt_states batt_state;  // state: charged, low, or drained
  float batt_voltage;  // state: voltage
} umbelt_batt;

static umbelt_batt s_batt = {.batt_initialized = false};


void battery_init() {
  s_batt.batt_initialized = true;
  pinMode(PIN_VBAT, INPUT);
}

void check_battery() {
  if (!s_batt.batt_initialized)
    battery_init();  // initialized module at startup
  else if (millis() - s_batt.last_read_time < BATT_UPDATE_INTERVAL)
    return;  // short circuit if not enough time has elapsed
  s_batt.last_read_time = millis();

  int analogCode = analogRead(PIN_VBAT);
  float voltage = (float) analogCode / ANALOG_BITS * IO_VOLTAGE;
  s_batt.batt_voltage = voltage * 2;  // correct for 1/2 voltage divider
  if (s_batt.batt_voltage < VOLTAGE_DRAINED_THRESHOLD)
    s_batt.batt_state = BATT_DRAINED;
  else if (s_batt.batt_voltage < VOLTAGE_LOW_THRESHOLD)
    s_batt.batt_state = BATT_LOW;
  else
    s_batt.batt_state = BATT_CHARGED;
#ifdef BATT_DEBUG
  Serial.print("Battery voltage: ");
  Serial.println(s_batt.batt_voltage);
#endif
}

batt_states get_battery_state() {
  return s_batt.batt_state;
}

float get_battery_voltage() {
  return s_batt.batt_voltage;
}
