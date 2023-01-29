#include <Arduino.h>

#include "umbelt_battery.h"

#include "umbelt_haptics.h"
#include "umbelt_haptic_patterns.h"

#define BATT_DEBUG 1
#define ANALOG_BITS 1024
#define IO_VOLTAGE 3.3
#define BATT_UPDATE_INTERVAL 5000  // update each second

#define VOLTAGE_MAX 4.2
#define VOLTAGE_LOW_THRESHOLD 3.7
#define VOLTAGE_DRAINED_THRESHOLD 3.6
#define VOLTAGE_NO_BATT_THRESHOLD 0.25
#define VOLTAGE_FUDGE_FACTOR .35  // amount measurement undershoots real voltage by


typedef struct umbelt_batt {
  bool batt_initialized;  // is the battery module initialized?
  long long last_read_time;  // last time state was updated
  batt_states batt_state;  // state: charged, low, or drained
  float batt_voltage;  // state: voltage
} umbelt_batt;

static umbelt_batt s_batt = {.batt_initialized = false};

void battery_warn();
void print_battery();

void battery_init() {
  s_batt.batt_initialized = true;
  pinMode(PIN_VBAT, INPUT);
}

void battery_tick() {
  if (!s_batt.batt_initialized)
    battery_init();  // initialized module at startup
  else if (millis() - s_batt.last_read_time < BATT_UPDATE_INTERVAL)
    return;  // short circuit if not enough time has elapsed
  s_batt.last_read_time = millis();

  int analogCode = analogRead(PIN_VBAT);
  float voltage = ((float) analogCode / ANALOG_BITS * IO_VOLTAGE);
  s_batt.batt_voltage = voltage * 2 + VOLTAGE_FUDGE_FACTOR;  // correct for 1/2 voltage divider
  if (s_batt.batt_voltage < VOLTAGE_NO_BATT_THRESHOLD)
    s_batt.batt_state = NO_BATT;
  else if (s_batt.batt_voltage < VOLTAGE_DRAINED_THRESHOLD)
    s_batt.batt_state = BATT_DRAINED;
  else if (s_batt.batt_voltage < VOLTAGE_LOW_THRESHOLD)
    s_batt.batt_state = BATT_LOW;
  else
    s_batt.batt_state = BATT_CHARGED;
  if (BATT_DEBUG) print_battery();
  battery_warn();
}

void battery_warn() {
  /* Warn the wearer that the battery voltage is getting low if it is. */
  if (s_batt.batt_state != BATT_DRAINED  // don't warn if not low
      && s_batt.batt_state != BATT_LOW) return;
  run_haptics(batt_warn_pulse, /*num_pulses=*/6, /*motor_idx=*/0,
              /*intensity=*/1);
}

batt_states get_battery_state() {
  return s_batt.batt_state;
}

float get_battery_voltage() {
  return s_batt.batt_voltage;
}

void print_battery() {
  Serial.println("========== BATTERY ==========");
  Serial.print("Voltage: ");
  Serial.print(s_batt.batt_voltage);
  Serial.print(", State: ");
  switch(s_batt.batt_state) {
    case BATT_CHARGED:
      Serial.print("CHARGED");
      break;
   case BATT_LOW:
      Serial.print("LOW");
      break;
   case BATT_DRAINED:
      Serial.print("DRAINED");
      break;
   case NO_BATT:
      Serial.print("NO BATT");
      break;
   default:
      Serial.print("UNKNOWN");
  }
  Serial.println(".");
  Serial.print("Initialized: ");
  Serial.print(s_batt.batt_initialized, DEC);
  Serial.print(", Last Read Time: ");
  Serial.print((double)s_batt.last_read_time / 1000);
  Serial.println(" sec.");
  Serial.println("========== END BATTERY ==========");
}
