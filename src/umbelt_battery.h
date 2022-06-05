/* Battery management module. Main function is to monitor the battery voltage
 * and stop power-hungry functions when the battery is drained.
 */

typedef enum {
  BATT_CHARGED,
  BATT_LOW,
  BATT_DRAINED
} batt_states;


/* Updates the known battery voltage and state by reading from the battery
 * pin. */
void check_battery();

/* Returns the known battery state. Does not read the battery state. */
batt_states get_battery_state();

/* Returns the known battery voltage. Does not read the battery voltage. */
float get_battery_voltage();
