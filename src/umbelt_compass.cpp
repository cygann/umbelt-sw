#include <Arduino.h>

#include "umbelt_compass.h"
#include "umbelt_haptics.h"
#include "umbelt_common.h"
#include "mmc5633.h"

/* Struct to represent an instance of this module.
 * and gyroscope devices.
 *
 * enabled : Whether the compass module is enabled.
 * magneto : Handle to magnetometer object.
 * heading : Current heading, in degrees.
 * last_update_heading : Previous heading that was vibrated.
 * last_update_motor : Previous motor that was vibrated.
 * last_update_time : Time in ms since last haptic update.
 * magnetic_{x, y, z} : Current magnetometer readings for x, y, z coords.
 */
typedef struct umbelt_compass {
    bool          enabled;
    MMC5633       magneto;
    float         heading;
    float         last_update_heading;
    int           last_update_motor;
    unsigned long last_update_time;
    int           magnetic_x;
    int           magnetic_y;
    int           magnetic_z;
} umbelt_compass;

// Instance of this module.
static umbelt_compass s_compass;


/* ===================== Module-specific functions ========================= */

/* Determines if the current heading is greater than thresh degrees from the
 * last heading when a haptic vibration was produced.
 *
 * @param thresh : Threshold heading change is compared to.
 *
 * @return true is heading change is greater than thres, false otherwise.
 */
bool heading_change_greater_than(float thresh);

/* Reads from Magnetometer to get x, y, and z magentic fields, then resolves
 * the heading (in degrees). Updates the compass object to have this heading.
 */
void resolve_heading(void);

/* Debug printout for various compass fields. */
void compass_debug(void);


void umbelt_compass_init(bool enable_compass) {

  // Init compass module struct
  memset(&s_compass, 0, sizeof(umbelt_compass));

  s_compass.magneto = MMC5633();
  s_compass.heading = 0;
  s_compass.last_update_heading = 0;
  s_compass.last_update_time = 0;
  s_compass.last_update_motor = 0;

  s_compass.enabled = enable_compass;

}

void enable_compass(void) {
  s_compass.enabled = true;
}

void disable_compass(void) {
  s_compass.enabled = false;
}

void umbelt_compass_tick(void) {
    if (!s_compass.enabled) return;
    if (!(millis() > s_compass.last_update_time + 1000)) return;

    // Obtain current heading.
    resolve_heading();

    // Ensure the change in heading is significant enough to warrant haptics.
    if (!heading_change_greater_than(15)) return;
    int bin = 13 - (s_compass.heading * N_MOTORS) / 360;

    actuate_motor(bin, /*duration=*/ 80 /*ms*/, /*percent_motor=*/1);

    // Update this state as the previous one.
    s_compass.last_update_motor = bin;
    s_compass.last_update_time = millis();
    s_compass.last_update_heading = s_compass.heading;
}

bool heading_change_greater_than(float thresh) {
    float first_angle = abs(s_compass.heading - s_compass.last_update_heading);
    float second_angle = 360 - first_angle;
    float smaller = min(first_angle, second_angle);

    return smaller > thresh;
}

void resolve_heading(void) {

    int mag_x = 0;
    int mag_y = 0;
    int mag_z = 0;

    // Read from Magnetometer: use the average of SAMPLING_N different
    // measurements to denoise.
    for (int i = 0; i < SAMPLING_N; i++) {
        mag_reading reading;
        bool success = s_compass.magneto.read(&reading);
        mag_x += reading.mag_x;
        mag_y += reading.mag_y;
        mag_z += reading.mag_z;
    }
    s_compass.magnetic_x = (mag_x / SAMPLING_N) - X_OFFSET;
    s_compass.magnetic_y = (mag_y / SAMPLING_N) - Y_OFFSET;
    s_compass.magnetic_z = (mag_z / SAMPLING_N) - Z_OFFSET;

    // Calculate the angle of the vector x and z and resolve heading.
    float heading = (atan2(s_compass.magnetic_x,
                           s_compass.magnetic_z) * 180) / Pi;
    if (heading < 0) {
        heading = 360 + heading;
    }
    s_compass.heading = heading;
}

void compass_debug() {
    rtt.println("");
    rtt.print("Magnetic: ");
    rtt.print(s_compass.magnetic_x);
    rtt.print(" ");
    rtt.print(s_compass.magnetic_y);
    rtt.print(" ");
    rtt.print(s_compass.magnetic_z);
    rtt.print(" ");
    rtt.print("Compass Heading: ");
    rtt.print(s_compass.heading);
    rtt.print(" ");
    rtt.print("Motor: ");
    rtt.print(s_compass.last_update_motor);
}
