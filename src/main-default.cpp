#include <Arduino.h>

/* Umbelt modules */
#include "umbelt_ble.h"
#include "umbelt_common.h"
#include "battery.h"
#include "umbelt_haptics.h"
#include "umbelt_haptic_patterns.h"
#include "umbelt_compass.h"
#include "umbelt_clock.h"

#define BAT_PIN 20

#define UMBELT_COMPASS_ENABLE false

void setup(void) {
    rtt.println(" ======== Umbelt =====|=[] ");

    // Init umbelt modules
    umbelt_haptics_init();
    umbelt_compass_init(UMBELT_COMPASS_ENABLE);

    umbelt_ble_init();

    init_indicator_led();
}

void loop(void) {
    run_haptics(bach, /*num_pulses=*/32, /*motor_idx=*/0, /*intensity=*/0.85);
    umbelt_compass_tick();
    umbelt_ble_tick();
    check_battery();
}
