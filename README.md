
# Data Acquisition Computer.



This repository contains firmware for a Data Acquisition Computer that I am making. 

I am using STM32 series of chips for this flight computer. 

Initialization code has been generated using STM32CubeMXIDE.

I am integrating the following sensors with an STM32G0 chip :
| Sr. No | Sensor    | Functionality          | Protocol     |
| ------ | --------- | ---------------------- | ------------ |
| 1      | BMP390    | Pressure               | I2C          |
| 2      | ICM-20600 | Accelerometer (3-axis) | I2C          |
|        |           | Gyro (3-axis)          | I2C          |
| 3      | LoRa      | RFM95CW                | SPI          |
| 4      | GPS       | NEO6M                  | UART/SPI/I2C |


Firmware for each of the sensors is in their own folder in `Firmware/Drivers/Sensors` folder. 
