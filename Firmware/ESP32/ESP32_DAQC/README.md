# ESP32 Firmware 


Firmware in this repository is made for Espressif ESP32-S3 chip with the following specifications .

| Memory           | GPIO                   | Interfaces | WiFi                                     | Bluetooth                    |
| ---------------- | ---------------------- | ---------- | ---------------------------------------- | ---------------------------- |
| 384KB ROM        | 4 Strapping GPIO       | 2x I2C     | IEEE 802.11 b/g/n 2.4GHz HT20/40 150Mbps | BLE v5.O (Certified to v5.3) |
| 512KB SRAM       | 6 GPIO for flash/PSRAM | 2x I2S     |                                          |                              |
| 16KB  RTCSRAM    |                        | 4x SPI     |                                          |                              |
| 8MB Flash Memory |                        | 3x UART    |                                          |                              |
|                  |                        | 1x USB OTG |                                          |                              |

**Part Number** : `ESP32-S3FN8`
All technical reference manuals, datasheets and hardware design guidlines can be found [here](https://products.espressif.com/#/product-selector?language=en&names=)

This chip will be interfacing with the following sensors :

| Sr. No | Sensor  | Functionality          | Protocol     | Status |
| ------ | ------- | ---------------------- | ------------ | ------ |
| 1      | BMP390  | Pressure               | I2C          | Done   |
| 2      | ADXL375 | Accelerometer (3-axis) | I2C          | Done   |
|        |         | Gyro (3-axis)          | I2C          |        |
| 3      | LoRa    | RFM95CW                | SPI          |        |
| 4      | GPS     | NEO6M                  | UART/SPI/I2C |        |


