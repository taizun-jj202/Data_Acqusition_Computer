# DEVNOTES 

Intended to be a file for notekeeping for different sensors.


## ADXL375 :


### Notes :
- Tie CS_bar pin to V_dd/io to enable I2C mode.
- When `ALT_ADDRESS` = 1, I2C Address = `0x1D`
- DEVID @ (0x00) = `0xE5` 
- Set POWER_CTL @ (0x2D)  = 0x08 (Enable `Measure` bit)
- 
### Dependancies :
- `Wire.h` library for I2C comm
  