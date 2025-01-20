/**
 * @file Main application for ESP32 Flight Data Capture Computer 
 * 
 * This file contains main application code for ESP32 data acquisition computer. 
 * This file is experimental code to test data capture system before developing 
 *  a more advanced sensor suite code.
 * 
 * This board is supposed to capture the following data :
 *  
 * | Sr. No | Data        | Type of data | Sensor        | Code Status |
 * | ------ | ----------- | ------------ | ------------- | ----------- |
 * | 1      | Altitude    | Barometer    | BMP390/BMP388 |             |
 * | 2      | Roll, Pitch | Accerelomter | ADXL375       | Done        |
 * | 3      | GPS         | GPS          | Ublox NEO-7M  |             |
 * 
 * ------------------------------------------------------------------------
 *          ADXL375 Accelerometer
 * ------------------------------------------------------------------------
 * Sensor ID in code : 0200  
 * Sensor pins : SDA -> 21
 *               SCL -> 22
 * Sensor protocol : I2C
 * 
 * 
 * 
 * @author Taizun Jafri (jafri.taizun.s@gmail.com)
 * @date 19/01/2025
 */


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADXL375.h>
#include <stdint.h>


// Defines
#define I2C_SDA 21                    // I2C SDA Line            
#define I2C_SCL 22                    // I2C SCL Line 
#define ADXL375_I2C_ADDRESS (0x53)    // ADXL375 I2C address
#define ADXL375_DATA_X0_REG (0x32)    // This and next 5 regs contain X,Y,Z acceleration respectively.



//-----------------------------------------------------------------------
// ADXL375 Function declarations
//-----------------------------------------------------------------------
Adafruit_ADXL375 High_G_accelerometer = Adafruit_ADXL375(0200);  // Defining ADXL375 object
void ADXL375_init(void);                                         // Initialize ADXL375
void ADXL375_acc_in_G (
             int16_t *x,
             int16_t *y,
             int16_t *z
);                                                               // Get acceleration data in G


void setup() {

  Serial.begin(115200);
  Wire.begin( I2C_SDA, I2C_SCL); // Use this I2C interface instead of default. 
}

void loop() {
  // put your main code here, to run repeatedly:
}


//-----------------------------------------------------------------------
// ADXL375 Function Definitions :
//-----------------------------------------------------------------------
void ADXL375_init(void){
  if(!High_G_accelerometer.begin()){
    Serial.println("\n ADXL375 not found...");
  }
  else{
    Serial.println("Initializing ADXL375 Accelerometer");
  }
}

void ADXL375_acc_in_G (
                       int16_t *x,
                       int16_t *y,
                       int16_t *z
){

  uint8_t raw_acc_data[6] = {0};

  // Starting I2C transmission.
  Wire.beginTransmission(ADXL375_I2C_ADDRESS);
  Wire.write(ADXL375_DATA_X0_REG);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL375_I2C_ADDRESS, 6);
  for (int i = 0; i < 6; i++) {
    raw_acc_data[i] = Wire.read();
  }

  *x = (int16_t)((raw_acc_data[1] << 8) | (raw_acc_data[0]) );
  *y = (int16_t)((raw_acc_data[3] << 8) | (raw_acc_data[2]) );
  *z = (int16_t)((raw_acc_data[5] << 8) | (raw_acc_data[4]) );

}
