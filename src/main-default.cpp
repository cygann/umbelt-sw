#include <Arduino.h>

/* Umbelt modules */
#include "umbelt_ble.h"
#include "umbelt_common.h"
#include "umbelt_haptics.h"
#include "umbelt_haptic_patterns.h"
#include "umbelt_compass.h"
#include "umbelt_clock.h"

#define BAT_PIN 20

#define UMBELT_COMPASS_ENABLE false

void setup(void) {
    rtt.println(" ======== Umbelt =====|=[] ");

    // Init umbelt modules
    init_haptics();
    init_indicator_led();
    umbelt_compass_init(UMBELT_COMPASS_ENABLE);
}

void loop(void) {
    run_haptics(bach, /*num_pulses=*/32, /*motor_idx=*/0, /*intensity=*/0.85);
    umbelt_compass_tick();
}
