#include <Wire.h>

#include "MMC5633.h"

/* Struct to represent an instance of this module.
 *
 * continuous_mode : Whether the sensor is in continous measurement mode.
 */
typedef struct mmc5633 {
  bool continuous_mode;
} mmc5633;

// Instance of this module.
static mmc5633 s_mmc5633;


/* Sets whether continous measurement mode should be enabled on the sensor.
 *
 * @param continuous : Whether continous mode should be set.
 */
void set_continuous_mode(bool continous);

/* Initiates a read of NUM_BYTES from the MMC5633 at a particular register.
 * Calling function should use Wire.read() to read out the bytes.
 *
 * TODO: probably just make this read the bytes into a buffer.
 *
 * @param reg : Register address to read from.
 * @param num_bytes : Number of bytes to read.
 */
void read_from_reg(uint8_t reg, uint8_t num_bytes);


void init_mmc5633(void) {
  memset(&s_mmc5633, 0, sizeof(mmc5633));
  s_mmc5633.continuous_mode = false;
}

bool mmc5633_get_reading(mag_reading *reading) {

    Wire.begin();

    // If not continous mode, we must first check if reading is ready.
    if (!s_mmc5633.continuous_mode) {
        Wire.beginTransmission(MMC5633_I2C_ADDR);
        Wire.write(MMC5633_REG_CTRL_0);
        Wire.write(MMC5633_BYTE_READ);
        Wire.endTransmission();

        // Wait until ready
        bool ready = false;
        while (!ready) {
            read_from_reg(MMC5633_REG_STATUS_1, 1);
            ready = Wire.read() & MMC5633_MEAS_M_DONE;
            delay(10);
        }
    }

    // begin trans
    // mag addr, write
    // device status register 1
    // data appears on SDA, done or not

    // Indicate read from XOUT_0 register
    Wire.beginTransmission(MMC5633_I2C_ADDR);
    Wire.write(MMC5633_REG_XOUT0);
    Wire.endTransmission(false);
    // TODO repeated start?

    // Request 6 bytes from MMC5633:
    // First two bytes: XOUT_0, XOUT_1
    // Next two bytes: YOUT_0, YOUT_1
    // Final two bytes: ZOUT_0, ZOUT_1
    int bytes = Wire.requestFrom(MMC5633_I2C_ADDR, 6);
    delay(8);

    int32_t mag_x_b, mag_y_b, mag_z_b;
    if (Wire.available() >= 6) {
       mag_x_b = Wire.read();
       mag_x_b = mag_x_b << 8;
       mag_x_b |= Wire.read();
       mag_x_b = mag_x_b - (2 << 14);

       mag_y_b = Wire.read();
       mag_y_b = mag_y_b << 8;
       mag_y_b |= Wire.read();
       mag_y_b = mag_y_b - (2 << 14);

       mag_z_b = Wire.read();
       mag_z_b = mag_z_b << 8;
       mag_z_b |= Wire.read();
       mag_z_b = mag_z_b - (2 << 14);
    } else {
        // Serial.println("No reading");
        return false;
    }

    // float mag_x = (float) mag_x_b;
    // float mag_y = (float) mag_y_b;
    // float mag_z = (float) mag_z_b;

    String reading_str = "X: " + String(mag_x_b) + " Y: " + String(mag_y_b) +
            " Z: " + String(mag_z_b);

    reading->mag_x = mag_x_b;
    reading->mag_y = mag_y_b;
    reading->mag_z = mag_z_b;

    // Serial.println(reading_str);

    // CMM continuous mode
    // write # to ODR[7:0]
    // CMM freq en set to 1
    return true;
}

void mmc5633_set_continous_mode(bool continous) {
  // TODO implement this.
}

void read_from_reg(uint8_t reg, uint8_t num_bytes) {
    Wire.beginTransmission(MMC5633_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission(); // false for repeated start
    Wire.requestFrom(MMC5633_I2C_ADDR, num_bytes);
}

bool motion_detected(void) {
    read_from_reg(MMC5633_REG_STATUS_1, 1);
    return Wire.read() & MMC5633_MOTION_DETECTED;
}

