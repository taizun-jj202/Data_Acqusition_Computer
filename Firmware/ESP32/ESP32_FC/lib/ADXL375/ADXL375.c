
#include "ADXL375.h"

int16_t read16(uint8_t reg) {
  Wire.beginTransmission(ADXL375_ADDRESS);
  i2cwrite(reg);
  Wire.endTransmission();
  Wire.requestFrom(ADXL375_ADDRESS, 2);
  return (uint16_t)(i2cread() | (i2cread() << 8));
}

/**************************************************************************/
/*!
    @brief  Gets the most recent X axis value
*/
/**************************************************************************/
int16_t getX(void) {
  return read16(ADXL375_DATAX0_REG);
}

/**************************************************************************/
/*!
    @brief  Gets the most recent Y axis value
*/
/**************************************************************************/
int16_t getY(void) {
  return read16(ADXL375_DATAY0_REG);
}

/**************************************************************************/
/*!
    @brief  Gets the most recent Z axis value
*/
/**************************************************************************/
int16_t getZ(void) {
  return read16(ADXL375_DATAZ0_REG);
}

inline void i2cwrite(uint8_t x) {
  #if ARDUINO >= 100
    Wire.write((uint8_t)x);
  #else
    Wire.send(x);
  #endif
}

inline uint8_t i2cread(void) {
  #if ARDUINO >= 100
    return Wire.read();
  #else
    return Wire.receive();
  #endif
}

void writeTo(int device, byte address, byte val) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        // send register address
  Wire.write(val);        // send value to write
  Wire.endTransmission(); //end transmission
}

void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission

  Wire.beginTransmission(device); //start transmission to device (initiate again)
  Wire.requestFrom(device, num);    // request num bytes from device

  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}

