/**
 * @file main.cpp
 * @brief Main firmware for ESP32 DAQC computer.
 * 
 * This file contains the main firmware for the ESP32 DAQC computer.
 * Firmware has the following functionalities:
 * - Initialize all sensors.
 * - Read data from sensors.
 * - Store data either on built-in flash memory or SD card. 
 *    - SD card data storage is prioritized over built-in flash memory.
 */

#include <Arduino.h>
#include "adxl375.h"
#include "bmp390.h"



void setup() {

  Serial.begin(9600); // Enabling serial communication
  ADXL375_init(); // Initializing the ADXL375 accelerometer
  // bmp390_init(); // Initializing the BMP390 barometric pressure sensor

}

// Initialization variables :
    // ADXL375 accelerometer variables. 
  int16_t x_acc;
  int16_t y_acc;
  int16_t z_acc;

void loop() {
  

  // Reading ADX375 accelerometer data
  ADXL375_read_acceleration(&x_acc, &y_acc, &z_acc);

}

