/**
 * @file bmp390_sensor.c
 * @brief Function definitions to enable BMP390 sensor.
 *
 * This file contains the implementation of functions required to initialize,
 * configure, and read data from the BMP390 sensor. The BMP390 is a high-precision
 * barometric pressure sensor that can be used for various applications such as
 * altitude measurement, weather monitoring, and indoor navigation.
 *
 * The functions provided in this file include:
 * - Initialization of the BMP390 sensor
 * - Configuration of sensor settings
 * - Reading pressure and temperature data from the sensor
 *
 * Usage:
 * 1. Initialize the sensor using the bmp390_init() function.
 * 2. Configure the sensor settings as needed.
 * 3. Read pressure and temperature data using the provided functions.
 *
 * Example:
 * @code
 * bmp390_init();
 * bmp390_configure();
 * float pressure = bmp390_read_pressure();
 * float temperature = bmp390_read_temperature();
 * @endcode
 *
 * @note Ensure that the I2C or SPI communication is properly set up before
 *       using these functions.
 *
 * @author Taizun Jafri 
 * @date 28-12-2024
 */


/****************************************
 * USER INCLUDES
 * **************************************/
#include <stdio.h>
#include <Wire.h>
#include "bmp390.h"




/****************************************
 * BMP390 FUNCTION DEFINITIONS
 * **************************************/


/**
 * @brief Initialize the BMP390 sensor.
 */
void bmp390_init(){
    Wire.begin();
    BMP390_write(BMP390_PWR_CTRL, 0x33);  // Normal mode, pressure and temperature measurement enabled
    BMP390_write(BMP390_OSR, 0x02);       // Set oversampling rate to x32 for pressure and x4 for temperature
    BMP390_write(BMP390_ODR, 0x02);       // Set output data rate to 50Hz
    BMP390_write(BMP390_CONFIG, 0x00);    // Set IIR filter coefficient to 0. Bypass filtering and get raw data values. 

}



/**
 * @brief Writes values to the BMP390 sensor registers when using I2C.
 * @param[in] reg Register address to write to
 * @param[in] value Value to write to the register
 */
void BMP390_write(uint8_t reg, uint8_t value){
    Wire.beginTransmission(BMP390_I2C_ADDR_1);
    Wire.write(reg); // Write register address
    Wire.write(value); // Write value to register
    Wire.endTransmission();

}


/**
 * @brief Reads values from the BMP390 sensor registers when using I2C.
 * @param[in] reg Register address to read from
 * @param[in] value Pointer to variable where value read from register will be stored
 * @param[in] number_of_bytes Number of bytes to read
 */
void BMP390_read(uint8_t reg, uint8_t *value, uint8_t number_of_bytes){
    Wire.beginTransmission(BMP390_I2C_ADDR_1);
    Wire.write(reg); // Write register address
    Wire.requestFrom(BMP390_I2C_ADDR_1, number_of_bytes); // Request 1 byte
    *value = Wire.read(); // Read value
    Wire.endTransmission(true);
}


/**
 * @brief Read the pressure from the BMP390 sensor
 * @param[out] pressure Pointer to variable to store pressure data
 * @note data[2] contains the MSB, hence is shifted 16bits to left.
 */
void BMP390_read_pressure(uint32_t *pressure){
    uint8_t data[3];
    BMP390_read(BMP390_PRESSURE_DATA, data, 3);
    *pressure = (uint32_t)data[2] << 16 | (uint32_t)data[1] << 8 | (uint32_t)data[0];
}


/**
 * @brief Read the temperature from the BMP390 sensor
 * @param[out] temperature Pointer to variable to store temperature data
 * @note data[2] contains the MSB, hence is shifted 16bits to left.
 */
void BMP390_read_temperature(uint32_t *temperature){
    uint8_t data[3];
    BMP390_read(BMP390_TEMPERATURE_DATA, data, 3);
    *temperature = (uint32_t)data[2] << 16 | (uint32_t)data[1] << 8 | (uint32_t)data[0];
}