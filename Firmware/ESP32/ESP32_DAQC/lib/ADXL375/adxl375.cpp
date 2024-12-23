#include "adxl375.h"
#include <Wire.h>

/**
 * @brief Initialize the ADXL375 accelerometer
 */
void ADXL375_init() {
  Wire.begin();
  ADXL375_write(ADXL_POWER_CTL, 0x08); // Set to measure mode
  ADXL375_write(ADXL_DATA_FORMAT, 0x0B); // Set to full resolution
  ADXL375_write(ADXL_BW_RATE, 0x0A); // Set to 100Hz
}

/**
 * @brief Write to register of ADLX375
 * @param[in] reg Register to be written to in ADXL375
 * @param[in] value Value to be written to register in ADXL375
 * @return None
 */
void ADXL375_write(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(ADXL_ADDR);
  Wire.write(reg); // Write register address
  Wire.write(value); // Write value to register
  Wire.endTransmission();
}

/**
 * @brief Read from register of ADLX375
 * 
 * This fucnction communicates with the ADXL375 accelerometer sensor to 
 * read data from the given input register to the given output pointer 
 * variable where data is stored. 
 *   
 * @param[in] reg Register to be read from in ADXL375
 * @param[in] value Pointer to variable where value read from register will be stored
 * @param[in] bytes Number of bytes to read. 
 * @return Value read from register
 */
void ADXL375_read(uint8_t reg, uint8_t *value, uint8_t number_of_bytes) {
  Wire.beginTransmission(ADXL_ADDR);
  Wire.write(reg); // Write register address
  Wire.endTransmission();
  Wire.requestFrom(ADXL_ADDR, number_of_bytes); // Request 1 byte
  *value = Wire.read(); // Read value
}


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
void ADXL375_read_acceleration(int16_t *x, int16_t *y, int16_t *z) {
  uint8_t data[6];
  ADXL375_read(ADXL_DATAX0, data, 6);
  *x = (int16_t)(data[1] << 8 | data[0]);
  *y = (int16_t)(data[3] << 8 | data[2]);
  *z = (int16_t)(data[5] << 8 | data[4]);
}