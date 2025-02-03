

# ESP32_FC 

This repo contains code for ESP32 flight data logger. 

The ESP32 interfaces with the following sensors :

1. [ADXL375 Accelerometer](#adxl375-) 
2. [BMP390 Pressure Sensor](#bmp390-barometer)
3. [Ublox NEO-7M GPS Sensor](#ublox-neo-7m-gps-module)
4. [SD Card (Data storage device.)](#sd-card-details-)



---


### ADXL375 :

|                    |                                                                                         |
| ------------------ | --------------------------------------------------------------------------------------- |
| **Specifications** | 3-axis, +/- 200g Acceleration                                                           |
| **Interface used** | I2C                                                                                     |
| **Pins**           | SDA(21), SCL(22)                                                                        |
| **Datasheet**      | [Link](https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL375.PDF) |
| **Purchase Link**  |                                                                                         |



### BMP390 Barometer

|                    |                                                                                                        |
| ------------------ | ------------------------------------------------------------------------------------------------------ |
| **Specifications** | +/- 3Pa or +/- 0.25m Accuracy                                                                          |
| **Interface used** | I2C                                                                                                    |
| **Pins**           | SDA(21), SCL(22)                                                                                       |
| **Datasheet**      | [Link](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp390-ds002.pdf) |
| **Purchase Link**  |                                                                                                        |
|                    |                                                                                                        |


### Ublox NEO-7M GPS Module

|                    |                                                                                                                  |
| ------------------ | ---------------------------------------------------------------------------------------------------------------- |
| **Specifications** | 4m position accuracy,                                                                                            |
| **Interface used** | UART2                                                                                                            |
| **Pins**           | GPIO(16)/RX  , GPIO(17)/TX for Serial2                                                                           |
| **Datasheet**      | [Link](https://content.u-blox.com/sites/default/files/products/documents/NEO-7_DataSheet_%28UBX-13003830%29.pdf) |
| **Purchase Link**  |                                                                                                                  |
|                    |                                                                                                                  |


### SD Card Details : 

|                    |                                                                                                                  |
| ------------------ | ---------------------------------------------------------------------------------------------------------------- |
| **Specifications** | 16GB                                                                                                             |
| **Interface used** | SPI                                                                                                              |
| **Pins**           | GPIO12/SCK , GPIO11/MOSI , GPIO13/MISO , GPIO10/CSO                                                              |
| **Purchase Link**  |                                                                                                                  |s
