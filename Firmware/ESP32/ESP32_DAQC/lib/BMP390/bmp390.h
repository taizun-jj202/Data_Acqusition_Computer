/**
 * @file bmp390.h
 * @brief Function definitions for interfacing with the BMP390 pressure sensor on the ESP32 DAQC.
 * 
 * This header file contains the function declarations for initializing and interacting with the BMP390 
 * pressure sensor. The BMP390 is a high-precision sensor used to measure atmospheric pressure and 
 * temperature, which can be utilized to determine altitude changes. This sensor is particularly 
 * useful in applications such as rocket altitude measurement for certification projects.
 * 
 * The functions provided allow for initialization of the sensor, reading and writing to its registers, 
 * and obtaining pressure and temperature data.
 * 
 * Register addresses for the BMP390 sensor are defined to facilitate communication with the sensor.
 * 
 * @note Ensure proper initialization of the sensor before attempting to read data.
 * @version 1.0
 * @date 2024-12-28
 * @author Taizun Jafri
*/
 

#ifndef BMP390_H
#define BMP390_H
#include <stdio.h>


// Define all register addresses for the BMP390 sensor
#define BMP390_CHIP_ID 0x00          // Chip ID register
#define BMP390_ERR_REG 0x02          // Error register
#define BMP390_STATUS 0x03           // Status register
#define BMP390_PRESSURE_DATA 0x04    // Pressure data register
#define BMP390_TEMPERATURE_DATA 0x07 // Temperature data register
#define BMP390_SENSOR_TIME 0x0C      // Sensor time register
#define BMP390_EVENT 0x10            // Event register
#define BMP390_INT_STATUS 0x11       // Interrupt status register
#define BMP390_FIFO_LENGTH 0x12      // FIFO length register
#define BMP390_FIFO_DATA 0x14        // FIFO data register
#define BMP390_FIFO_WTM_0 0x15       // FIFO watermark 0 register
#define BMP390_FIFO_WTM_1 0x16       // FIFO watermark 1 register
#define BMP390_FIFO_CONFIG_1 0x17    // FIFO configuration 1 register
#define BMP390_FIFO_CONFIG_2 0x18    // FIFO configuration 2 register
#define BMP390_INT_CTRL 0x19         // Interrupt control register
#define BMP390_IF_CONF 0x1A          // Interface configuration register
#define BMP390_PWR_CTRL 0x1B         // Power control register
#define BMP390_OSR 0x1C              // Oversampling register
#define BMP390_ODR 0x1D              // Output data rate register
#define BMP390_CONFIG 0x1F           // Configuration register
#define BMP390_CALIB_DATA 0x31       // Calibration data register
#define BMP390_CMD 0x7E       


// BMP390 I2C Address
#define BMP390_I2C_ADDR_0 0x76 // I2C Address when SDO connected to GND
#define BMP390_I2C_ADDR_1 0x77 // I2C Address when SDO connected to V_dd_io          // Command register


/**
 * @brief Initialize the BMP390 pressure sensor
 */
void BMP390_init();

/**
 * @brief Write to register of BMP390
 * @param[in] reg Register to be written to in BMP390
 * @param[in] value Value to be written to register
 */ 
void BMP390_write(uint8_t reg, uint8_t value);

/**
 * @brief Read from register of BMP390
 * @param[in] reg Register to be read from in BMP390
 * @param[in] value Pointer to variable to store value read from register
 * @param[in] number_of_bytes Number of bytes to read
 */
void BMP390_read(uint8_t reg, uint8_t *value, uint8_t number_of_bytes);


/**
 * @brief Read the pressure from the BMP390 sensor
 * @param[out] pressure Pointer to variable to store pressure data
 */
void BMP390_read_pressure(uint32_t *pressure);

/**
 * @brief Read the temperature from the BMP390 sensor
 * @param[out] temperature Pointer to variable to store temperature data
 */
void BMP390_read_temperature(uint32_t *temperature);


#endif