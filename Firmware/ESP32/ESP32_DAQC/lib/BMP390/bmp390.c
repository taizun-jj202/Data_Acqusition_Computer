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