#include <Adafruit_LIS3MDL.h>
#include <umbelt_haptics.h>
#include <umbelt_clock.h>

void
clock_test() {
    for (int hour = 1; hour <= 12; hour++) {
        int motors[12] = {0};
        hour_to_motor_values(hour, motors);
    }
}

/* For the hour passed in (value 1 - 12, inclusive), this function modifies the
 * array of motors with the right analogWrite value.
 */
void
hour_to_motor_values(int hour, int motors[]) {
    for (int i = 0; i < 12; i++) motors[i] = 0;
    int val = 230;

    switch (hour) {
        case 1:
            motors[10] = val;
            break;
        case 2:
            motors[9] = val;
            break;
        case 3:
            motors[8] = val;
            break;
        case 4:
            motors[7] = val;
            break;
        case 5:
            motors[6] = val;
            break;
        case 6:
            motors[6] = val * (2.0 / 3.0);
            motors[5] = val * (2.0 / 3.0);
            break;
        case 7:
            motors[5] = val;
            break;
        case 8:
            motors[4] = val;
            break;
        case 9:
            motors[3] = val;
            break;
        case 10:
            motors[2] = val;
            break;
        case 11:
            motors[1] = val;
            break;
        case 12:
            motors[0] = val * (2.0 / 3.0);
            motors[11] = val * (2.0 / 3.0);
            break;
        default:
            break;
    }
}
