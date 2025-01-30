/**
 * @file Main application for ESP32 Flight Data Capture Computer 
 * 
 * This file contains main application code for ESP32 data acquisition computer. 
 * This file is experimental code to test data capture system before developing 
 *  a more advanced sensor suite code.
 * 
 * This board is supposed to capture the following data :
 *  
 * | Sr. No | Data        | Type of Sensor | Sensor        | Code Status |
 * | ------ | ----------- | -------------- | ------------- | ----------- |
 * | 1      | Altitude    | Barometer      | BMP390/BMP388 | Done        |
 * | 2      | Roll, Pitch | Accerelomter   | ADXL375       | Done        |
 * | 3      | GPS         | GPS            | Ublox NEO-7M  | Done        |
 * | 4      | Yaw         | Gyroscope      | LSM6DS3       |             |
 * | 5      | Speed       | Pitot Tube     |               |             |
 * 
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
 * ------------------------------------------------------------------------
 *          Ublox NEO-7M GPS 
 * ------------------------------------------------------------------------
 * 
 * GPS Sensor will use UART Port 2. (Serial 2) for data collection.
 * For debugging, we might use another Serial port if needed. 
 * 
 * 
 * @author Taizun Jafri (jafri.taizun.s@gmail.com)
 * @date 19/01/2025
 */


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADXL375.h>
#include <Adafruit_BMP3XX.h>
#include <UbxGps.h>        // UBX Protocol Library
#include <UbxGpsConfig.h> // UBX configuration library used for startup configuration for GPS
#include <UbxGpsNavPvt.h> // Configure NAVPVT message from GPS Module for UBX protocol.
#include <HardwareSerial.h>
#include <stdint.h>


// Defines
#define I2C_SDA 21                    // I2C SDA Line            
#define I2C_SCL 22                    // I2C SCL Line 
#define ADXL375_I2C_ADDRESS (0x53)    // ADXL375 I2C address
#define ADXL375_DATA_X0_REG (0x32)    // This and next 5 regs contain X,Y,Z acceleration respectively.
#define GPS_BAUDRATE (115200) // GPS Serial2 port baud rate
#define SERIAL_BAUDRATE (115200) // Serial monitor baud rate

HardwareSerial *gpsSerial = &Serial2;



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


//------------------------------------------------------------------------------------------------------
// Ublox NEO-7M GPS function declarations 
//------------------------------------------------------------------------------------------------------
/**
 * Init function :::
 *    Initialize GPS by initializaing Serial port 2. 
 *    Set baud rate -> 115200.
 *    Set message type to NAVPVT.
 *    Extract following values :
 *      - iTOW
 *      - year
 *      - month
 *      - day
 *      - hour
 *      - min 
 *      - sec
 *      - longitude
 *      - latitude
 *      - height
 */
void GPS_Init();
void GPS_Capture_data();
bool syncGPSmsg(uint8_t GPS_byte); // Function to sync UART message to correct UBX btye



/**
 * ------------------------------------------------------------------------------------------------------
 * GLOBAL Variables 
 * 
 * - These variables to be saved in the SD Card every cycle.
 * ------------------------------------------------------------------------------------------------------
 */
int16_t AccX, AccY, AccZ = {0};
double Pressure, Temperature = {0};     // Pressure Reg -> [23:0] i.e 3 bytes. So we take 4B to be safe
// GPS values : 
// Type         Name            Unit  Description (Scaling)
unsigned long   GPS_iTOW;       //  ms    GPS time of week of the navigation epoch. See the description of iTOW for details.
unsigned short  GPS_year;       //  y     Year (UTC)
unsigned char   GPS_month;      //  month Month, range 1..12 (UTC)
unsigned char   GPS_day;        //  d     Day of month, range 1..31 (UTC)
unsigned char   GPS_hour;       //  h     Hour of day, range 0..23 (UTC)
unsigned char   GPS_min;        //  min   Minute of hour, range 0..59 (UTC)
unsigned char   GPS_sec;        //  s     Seconds of minute, range 0..60 (UTC)
long            GPS_lon;        //  deg   Longitude (1e-7)
long            GPS_lat;        //  deg   Latitude (1e-7)
long            GPS_height;     //  mm    Height above Ellipsoid
  



void setup() {

  Serial.begin(SERIAL_BAUDRATE);          // 115200
  Serial2.begin(GPS_BAUDRATE);            // UART connection to GPS.
  Wire.begin(I2C_SDA, I2C_SCL);           // Use this I2C interface instead of default.

  // Initialize ADXL375 High-G Accelerometer.
  ADXL375_init();
  // Initialize Barometer for Pressure and Temp measurement
  BMP390_init();
  // GPS Initialization : 
  GPS_Init();




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
  );

  // Capture and parse GPS data :
  GPS_Capture_data();

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

//------------------------------------------------------------------------------------------------------
// Ublox NEO-7M GPS Function declarations : 
//------------------------------------------------------------------------------------------------------
void GPS_Init(){
  // Serial.begin(GPS_BAUDRATE);
  Serial.println("Initializing GPS module...");
  UbxGpsConfig<HardwareSerial, HardwareSerial> *ubxGpsConfig = new UbxGpsConfig<HardwareSerial, HardwareSerial>(Serial2, Serial1);
  ubxGpsConfig->setBaudrate(GPS_BAUDRATE);
  ubxGpsConfig->setMessage(UbxGpsConfigMessage::NavPvt);
  ubxGpsConfig->setRate(100); //Set rate to 10Hz.
  ubxGpsConfig->configure();
   
}

void GPS_Capture_data() {
  // Process only the available GPS bytes without waiting
  while (gpsSerial->available()) {
    uint8_t GPSbyte = gpsSerial->read();
    
    // Process the received byte without blocking
    if (syncGPSmsg(GPSbyte)) {
      // GPS message successfully synced and parsed
      Serial.println("GPS message successfully synced and parsed.");
    }
  }
}

// Sync Message and fill buffer.
bool syncGPSmsg(uint8_t GPS_byte){

  static uint8_t buffer[92]; // 84B NAVPT msg + 2B for UBX header
  static uint8_t index = 0;
  static bool msgSync = false;

  if (!msgSync){
    
    // Check first two bytes for msg sync else start sync all over again
    if ( index == 0 && GPS_byte == 0xB5 ){
       buffer[index++] = GPS_byte;
    } 
    else if ( index == 1 && GPS_byte == 0x65 ){
      buffer[index++] = GPS_byte;
      msgSync = true;
    }
    else {
      index = 0 ;
    }

    return false; 
  }

  // Read a byte for every iteration of this loop.
  buffer[index++] = GPS_byte;

  if ( index == 92 ){
    
    // Reset index and message now becomes out of sync
    index = 0;
    msgSync = false;

    // Validate message and store message :
    if (buffer[2] == 0x01 && buffer[3] == 0x07) { // NAV-PVT header message 
      GPS_iTOW   = *((uint32_t *)&buffer[6]);
      GPS_year   = *((uint16_t *)&buffer[10]);
      GPS_month  = *((uint8_t  *)&buffer[12]);
      GPS_day    = *((uint8_t  *)&buffer[13]);
      GPS_hour   = *((uint8_t  *)&buffer[14]);
      GPS_min    = *((uint8_t  *)&buffer[15]);
      GPS_sec    = *((uint8_t  *)&buffer[16]);
      GPS_lon    = *((uint32_t *)&buffer[30]);
      GPS_lat    = *((uint32_t *)&buffer[34]);
      GPS_height = *((uint32_t *)&buffer[38]);
    }
  }

  return true;
}

