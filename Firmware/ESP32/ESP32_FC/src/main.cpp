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
 * ------------------------------------------------------------------------
 *          BMP390 Accelerometer
 * ------------------------------------------------------------------------
 * We get following parameters from BMP390 sensor :
 * 1. Pressure
 * 2. Temperature
 * 
 * Sensor gives out Pressure in Pascals(Pa).
 * Temperature is given out in  Celcius (C). 
 * 
 * 
 * Altitude is calculated from Pressure using the following formula :
 *      float atmospheric = readPressure() / 100.0F;
        return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));

 * Formula is taken from BMPXXX library. But we will calculate altitude 
 *  in post-procesing and not onboard the flight computer
 * 
 * 
 * @author Taizun Jafri (jafri.taizun.s@gmail.com)
 * @date 19/01/2025
 */


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADXL375.h>
#include <Adafruit_BMP3XX.h>
#include <stdint.h>


// Defines
#define I2C_SDA 21                    // I2C SDA Line            
#define I2C_SCL 22                    // I2C SCL Line 
#define ADXL375_I2C_ADDRESS (0x53)    // ADXL375 I2C address
#define ADXL375_DATA_X0_REG (0x32)    // This and next 5 regs contain X,Y,Z acceleration respectively.



//------------------------------------------------------------------------------------------------------
// ADXL375 Function declarations
//------------------------------------------------------------------------------------------------------
Adafruit_ADXL375 High_G_accelerometer = Adafruit_ADXL375(0200);  // Defining ADXL375 object
void ADXL375_init(void);                                         // Initialize ADXL375
void ADXL375_acc_in_G (
             int16_t *x,
             int16_t *y,
             int16_t *z
);                                                               // Get acceleration data in G


//------------------------------------------------------------------------------------------------------
// BMP390 Function declarations
//------------------------------------------------------------------------------------------------------
Adafruit_BMP3XX BMP390; //Creating BMP390 Instance.
void BMP390_init();
void BMP390_Pressure_Temp(
  double *Temp,
  double *Pressure
);


/**
 * ------------------------------------------------------------------------------------------------------
 * GLOBAL Variables 
 * 
 * - These variables to be saved in the SD Card every cycle.
 * ------------------------------------------------------------------------------------------------------
 */
int16_t AccX, AccY, AccZ = {0};
double Pressure, Temperature = {0};     // Pressure Reg -> [23:0] i.e 3 bytes. So we take 4B to be safe



void setup() {

  Serial.begin(115200);
  Wire.begin( I2C_SDA, I2C_SCL); // Use this I2C interface instead of default.

  // Initialize ADXL375 High-G Accelerometer.
  ADXL375_init();

  // Initialize Barometer for Pressure and Temp measurement
  BMP390_init();



}

void loop() {

  // Fetching Acceleration data and storing in Global variables
  ADXL375_acc_in_G ( 
    &AccX,
    &AccY,
    &AccZ
  );

  // Fetching Pressure and temperature data to Global buffers
  BMP390_Pressure_Temp (
    &Temperature,
    &Pressure
  )

}


//------------------------------------------------------------------------------------------------------
// ADXL375 Function Definitions :
//------------------------------------------------------------------------------------------------------
void ADXL375_init(void){
  if(!High_G_accelerometer.begin()){
    Serial.println("\n ADXL375 not found...");
  }
  else{
    Serial.println("Initializing ADXL375 Accelerometer to collect 3-axis acceleration data");
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

  *x = (((int16_t)((raw_acc_data[1] << 8) | (raw_acc_data[0]) )) * ADXL375_MG2G_MULTIPLIER) ; // Converting raw data to 'G' data via multiplier/
  *y = (((int16_t)((raw_acc_data[3] << 8) | (raw_acc_data[2]) )) * ADXL375_MG2G_MULTIPLIER);
  *z = (((int16_t)((raw_acc_data[5] << 8) | (raw_acc_data[4]) )) * ADXL375_MG2G_MULTIPLIER);

}


//------------------------------------------------------------------------------------------------------
// BMP390 Function Definitions :
//------------------------------------------------------------------------------------------------------

void BMP390_init () {

  if(!BMP390.begin_I2C()) {
    Serial.println(" BMP390 Barometer not found...");
  }
  else
  {
    Serial.println("Initializing BMP390 Barometer to collect pressure and temperature data");
  }


  BMP390.setOutputDataRate(BMP3_ODR_100_HZ);                   // Set output rate to 100Hz
  BMP390.setPressureOversampling(BMP3_OVERSAMPLING_4X);        // Standard resolution 18bit 0.66Pa accuracy
  BMP390.setTemperatureOversampling(BMP3_OVERSAMPLING_4X);     // 18bit/0.0012 C resolution.
  BMP390.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);           // Set IIR filter coefficient

  //Logging to be implemented :
  // 1. BMP390 Output rate : 100Hz
  // 2. Press oversampling set to : 4x -> 18b/0.66Pa
  // 3. Temp oversampling set to : 4x -> 18b/0.0012 C 

}


void BMP390_Pressure_Temp ( 
  double *Temp,
  double *Pressure
) {
  *Temp = BMP390.readTemperature();
  *Pressure = BMP390.readPressure();

}