# DEVNOTES 

Intended to be a file for notekeeping for different sensors.


## ADXL375 :


### Notes :
- Tie CS_bar pin to V_dd/io to enable I2C mode.
- Tie `ALT_ADDRESS` pin to `GND`
- When `ALT_ADDRESS` = 1, I2C Address = `0x1D` | `ALT_ADDRESS` = 0, I2C Address = `0x53`
- DEVID @ (0x00) = `0xE5` 
- Set POWER_CTL @ (0x2D)  = 0x08 (Enable `Measure` bit)
- 
### Dependancies :
- `Wire.h` library for I2C comm
  
Datasheet and code reference : 
- [Reference 1](https://howtomechatronics.com/tutorials/arduino/how-to-track-orientation-with-arduino-and-adxl345-accelerometer/) <br>
- [Reference 2](https://docs.arduino.cc/language-reference/en/functions/communication/wire/requestFrom/) <br>
**Driver Completed Date** : DEC23, 2024


## BMP390 :

### Notes :
- Connect SDO to V_dd_io so that I2C address is `0x77`.