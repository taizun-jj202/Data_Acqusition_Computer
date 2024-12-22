/**
 * @file adxl375.h
 * @brief Function definitions for ADXL374 in the ESP32 DAQC
 * 
 * This file contains the function definitions for the ADXL375 accelerometer
 */


#ifndef ADXL375_H
#define ADXL375_H
#include <Wire.h>

#define ADXL_ADDR 0x53            // ADXL375 I2C address
#define ADXL_DEVID 0x00           // Device ID
#define ADXL_THRESH_TAP 0x1D      // Tap threshold
#define ADXL_OFSX 0x1E            // X-axis offset
#define ADXL_OFSY 0x1F            // Y-axis offset
#define ADXL_OFSZ 0x20            // Z-axis offset
#define ADXL_DUR 0x21             // Tap duration
#define ADXL_LATENT 0x22          // Tap latency
#define ADXL_WINDOW 0x23          // Tap window
#define ADXL_THRESH_ACT 0x24      // Activity threshold
#define ADXL_THRESH_INACT 0x25    // Inactivity threshold
#define ADXL_TIME_INACT 0x26      // Inactivity time
#define ADXL_ACT_INACT_CTL 0x27   // Axis enable control for activity and inactivity detection
#define ADXL_THRESH_FF 0x28       // Free-fall threshold
#define ADXL_TIME_FF 0x29         // Free-fall time
#define ADXL_TAP_AXES 0x2A        // Axis control for tap/double tap
#define ADXL_ACT_TAP_STATUS 0x2B  // Source of tap/double tap
#define ADXL_BW_RATE 0x2C         // Data rate and power mode control
#define ADXL_POWER_CTL 0x2D       // Power-saving features control
#define ADXL_INT_ENABLE 0x2E      // Interrupt enable control
#define ADXL_INT_MAP 0x2F         // Interrupt mapping control
#define ADXL_INT_SOURCE 0x30      // Source of interrupts
#define ADXL_DATA_FORMAT 0x31     // Data format control
#define ADXL_DATAX0 0x32          // X-Axis Data 0
#define ADXL_DATAX1 0x33          // X-Axis Data 1
#define ADXL_DATAY0 0x34          // Y-Axis Data 0
#define ADXL_DATAY1 0x35          // Y-Axis Data 1
#define ADXL_DATAZ0 0x36          // Z-Axis Data 0
#define ADXL_DATAZ1 0x37          // Z-Axis Data 1
#define ADXL_FIFO_CTL 0x38        // FIFO control
#define ADXL_FIFO_STATUS 0x39     // FIFO status


/**
 * @brief Initialize the ADXL375 accelerometer
 */
void ADXL375_init();

/**
 * @brief Write to register of ADLX375
 * @param[in] reg Register to be written to in ADXL375
 * @param[in] value Value to be written to register
 */
void ADXL375_write(uint8_t reg, uint8_t value);


/**
 * @brief Read from register of ADLX375
 * @param[in] reg Register to be read from in ADXL375
 * @param[in] value Value to be read from register
 * @param[in] bytes Number of bytes to read
 * @return Value read from register
 */
void ADXL375_read(uint8_t reg, uint8_t *value, uint8_t bytes);

/**
 * @brief Reads the X, Y, and Z axis acceleration data from the ADXL375 sensor.
 *
 * This function communicates with the ADXL375 accelerometer sensor to retrieve
 * the acceleration data for the X, Y, and Z axes. The data is read from the
 * sensor's registers and stored in the provided variables.
 *
 * @param[out] x Pointer to a variable where the X-axis acceleration data will be stored.
 * @param[out] y Pointer to a variable where the Y-axis acceleration data will be stored.
 * @param[out] z Pointer to a variable where the Z-axis acceleration data will be stored.
 *
 * @return int Returns 0 on success, or a negative error code on failure.
 */
void ADXL375_read_acceleration(int16_t *x, int16_t *y, int16_t *z); 

#endif