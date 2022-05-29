/*  Umbelt Compass module.
 *  ----------------------
 */

const float Pi = 3.14159;

const int SAMPLING_N = 1; // Number of magnetometer measurements for denoising.

const float X_OFFSET = 0; // Constants from magnetometer calibration.
const float Y_OFFSET = 0;
const float Z_OFFSET = 0;

const float UPDATE_DUR = 200; // Time to for update haptics to persist, in ms

const float DEFAULT_UPDATE_THRESHOLD = 15; // In degrees, heading threshold
                                           // required for new haptic update.


/* Initializes the compass module by setting up magnetometer and enabling the
 * compass module.
 *
 * @param enable_compass : Whether to enable to compass module on init.
 */
void umbelt_compass_init(bool enable_compass);

/* Enable the compass module. */
void enable_compass(void);

/* Disable the compass module. */
void disable_compass(void);

/* Performs a single tick of the compass module. Reads the lastest data from
 * the magnetometer, resolves heading, and vibrates motors accordingly.
 */
void umbelt_compass_tick(void);
