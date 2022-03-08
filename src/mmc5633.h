#ifndef MMC5633_H
#define MMC5633_H

#include "Arduino.h"
// #include <Wire.h>

/* ================================= */
/*     I2C Addresses, registers      */
/* ================================= */

#define MMC5633_I2C_ADDR 0x30

#define MMC5633_REG_CTRL_0 0x1B // Init reading

#define MMC5633_REG_STATUS_1    0x18 // Status for reading ready
#define MMC5633_MEAS_M_DONE     0b01000000 // magnetic measurement done
#define MMC5633_MOTION_DETECTED 0b00000100

#define MMC5633_REG_ST_Y 0x28 // Test reg

#define MMC5633_REG_XOUT0 0x00  // X mag reading
#define MMC5633_REG_XOUT1 0x01
#define MMC5633_REG_YOUT0 0x02  // Y mag reading
#define MMC5633_REG_YOUT1 0x03
#define MMC5633_REG_ZOUT0 0x04  // Z mag reading
#define MMC5633_REG_ZOUT1 0x05
#define MMC5633_REG_XOUT2 0x06  // X additional
#define MMC5633_REG_YOUT2 0x07  // Y additional
#define MMC5633_REG_ZOUT2 0x08  // Z additional

// Code to write to MMC5633_REG_CTRL_0: sets TM_M (Take measure of mag field)
// and Auto_SR_en (enable auto set/reset) high
#define MMC5633_BYTE_READ 0b00100001

struct mag_reading {
    int16_t mag_x;
    int16_t mag_y;
    int16_t mag_z;
    unsigned long time; // Timestamp of reading
};

class MMC5633 {
    public:
        MMC5633(void);
        bool get_reading(mag_reading *reading);
        void set_continuous_mode();
        bool read(mag_reading *reading);
        bool motion_detected();
    private:
        void read_from_reg(uint8_t reg, uint8_t num_bytes);

        bool continuous_mode;

};

#endif
