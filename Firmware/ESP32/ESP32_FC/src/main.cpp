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
 * ------------------------------------------------------------------------
 *          SD Card Data storage
 * ------------------------------------------------------------------------
 * 
 * Avoid using following pins as they are being used internally for 
 *  SPI Flash and SRAM.
 * Pins to avoid :
 * - GPIO 26 (Flash/PSRAM SPICS1)
 * - GPIO 27 (Flash/PSRAM SPIHD)
 * - GPIO 28 (Flash/PSRAM SPIWP)
 * - GPIO 29 (Flash/PSRAM SPICS0)
 * - GPIO 30 (Flash/PSRAM SPICLK)
 * - GPIO 31 (Flash/PSRAM SPIQ)
 * - GPIO 32 (Flash/PSRAM SPID)
 * 
 * ESP32 has four SPI interfaces :
 * - SPI0 (Used Internally)(DO NOT USE) 
 * - SPI1 (Used Internally)(DO NOT USE)
 * - SPI2  
 * - SPI3 (Can be mapped to any four pins)
 * 
 * Plan to use SPI2. This channel has its own DMA channel. SPI3 has shared channel.
 * SPI2 (VSPI) :
 *  - GPIO 12 -> SCK
 *  - GPIO 11 -> MOSI
 *  - GPIO 13 -> MISO
 *  - GPIO 10 -> CSO
 * 
 * 
 * @author Taizun Jafri (jafri.taizun.s@gmail.com)
 * @date 19/01/2025
 */


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADXL375.h>
#include <Adafruit_BMP3XX.h>
#include <UbxGps.h>                   // UBX Protocol Library
#include <UbxGpsConfig.h>             // UBX configuration library used for startup configuration for GPS
#include <UbxGpsNavPvt.h>             // Configure NAVPVT message from GPS Module for UBX protocol.
#include <HardwareSerial.h>
#include <stdint.h>
#include <SD.h>
#include <SPI.h> 


// Defines
#define I2C_SDA 21                    // I2C SDA Line            
#define I2C_SCL 22                    // I2C SCL Line 
#define ADXL375_I2C_ADDRESS (0x53)    // ADXL375 I2C address
#define ADXL375_DATA_X0_REG (0x32)    // This and next 5 regs contain X,Y,Z acceleration respectively.
#define GPS_BAUDRATE (115200)         // GPS Serial2 port baud rate
#define SERIAL_BAUDRATE (115200)      // Serial monitor baud rate
// Any pins can be defined for SPI use. 
#define HSPI_MOSI  4                  // HSPI MOSI pin
#define HSPI_MISO  5                  // HSPI MISO pin
#define HSPI_SCK   6                  // HSPI SCK pin
#define HSPI_CS    7                  // HSPI Chip Select Pin


// Defining File for Data Logging
File DATA_LOG_FILE; // File Object for SD card file.

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


//------------------------------------------------------------------------------------------------------
// SD Card and SPI Interfaces
//------------------------------------------------------------------------------------------------------
/**
 * We use the following pins :
 * SPI2 (VSPI) :
 *  - GPIO 12 -> SCK
 *  - GPIO 11 -> MOSI
 *  - GPIO 13 -> MISO
 *  - GPIO 10 -> CSO
 */
void SD_Card_Init();
void SD_Save_Data();




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
// Dataframe separater so that dataframes can be separated.
uint8_t DATA_FRAME_START_END_BYTE = 0xFF;  



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
  // SD card Initialization
  SD_Card_Init();

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

  // Save data to SD Card:
  SD_Save_Data();

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

//------------------------------------------------------------------------------------------------------
// SD Card and SPI Interfaces
//------------------------------------------------------------------------------------------------------

void SD_Card_Init() {

  // Check for SD card initialization:
  if ( !SD.begin(HSPI_CS)){
    Serial.println("FAILED SD Card initialization...");
    return;
  }

  Serial.println("Initializing SD Card...");

  // Open file in append mode and file open check logic : 
  // Check for errors while opening file
  DATA_LOG_FILE = SD.open("/SENSOR_DATA.bin", FILE_APPEND);
  if (!DATA_LOG_FILE) {
    Serial.println("Error Opening file...");
  }else {
    Serial.println("File opened successfully");
  }

  DATA_LOG_FILE.close();


}

// Function to write binary data to SD card for speed purposes :
void SD_Save_Data () {


  DATA_LOG_FILE = SD.open("/sensor_data.csv", FILE_APPEND);
    if (DATA_LOG_FILE) {
        // Write raw numeric values, separated by commas (CSV-like format)
        // DATA_LOG_FILE.print(AccX); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(AccY); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(AccZ); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(Pressure, 2); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(Temperature, 2); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(GPS_iTOW); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(GPS_year); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(GPS_month); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(GPS_day); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(GPS_hour); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(GPS_min); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(GPS_sec); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(GPS_lon); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.print(GPS_lat); DATA_LOG_FILE.print(',');
        // DATA_LOG_FILE.println(GPS_height);  // End line

        // Write separater byte of Dataframe:
        DATA_LOG_FILE.write ( (uint8_t *)&DATA_FRAME_START_END_BYTE, sizeof(DATA_FRAME_START_END_BYTE));
        //Writing Acceleration (X,Y,Z) in binary format
        DATA_LOG_FILE.write ( (uint8_t *)&AccX, sizeof(AccX));
        DATA_LOG_FILE.write ( (uint8_t *)&AccY, sizeof(AccY));
        DATA_LOG_FILE.write ( (uint8_t *)&AccZ, sizeof(AccZ));
        // Writing Temp and Pressure
        DATA_LOG_FILE.write ( (uint8_t *)&Pressure, sizeof(Pressure));
        DATA_LOG_FILE.write ( (uint8_t *)&Temperature, sizeof(Temperature));
        // Writing GPS Data fields 
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_iTOW, sizeof(GPS_iTOW));
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_year, sizeof(GPS_year));
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_month, sizeof(GPS_month));
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_day, sizeof(GPS_day));
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_hour, sizeof(GPS_hour));
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_min, sizeof(GPS_min));
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_sec, sizeof(GPS_sec));
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_lon, sizeof(GPS_lon));
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_lat, sizeof(GPS_lat));
        DATA_LOG_FILE.write ( (uint8_t *)&GPS_height, sizeof(GPS_height));

        // Dataframe containing data ends with custom dataframe end i.e 0xFF.
        DATA_LOG_FILE.write ( (uint8_t *)&DATA_FRAME_START_END_BYTE, sizeof(DATA_FRAME_START_END_BYTE));


        // Troubleshooting code : 
        // fprintf(&DATA_LOG_FILE, "%lf,%lf,%ld,%ld,%lf\n", Pressure, Temperature, GPS_lon, GPS_lat, GPS_height);


        DATA_LOG_FILE.close();
    }

}