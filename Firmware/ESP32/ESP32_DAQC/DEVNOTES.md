# DEVNOTES 

Intended to be a file for notekeeping for different sensors.


## ADXL375 :


### Hardware Design notes :
- Tie CS_bar pin to V_dd/io to enable I2C mode.
- Tie `ALT_ADDRESS` pin to `GND`
- When `ALT_ADDRESS` = 1, I2C Address = `0x1D` | `ALT_ADDRESS` = 0, I2C Address = `0x53`
- DEVID @ (0x00) = `0xE5` 
- Set POWER_CTL @ (0x2D)  = 0x08 (Enable `Measure` bit)
- 
### Software Dependancies :
- `Wire.h` library for I2C comm
  
Datasheet and code reference : 
- [Reference 1](https://howtomechatronics.com/tutorials/arduino/how-to-track-orientation-with-arduino-and-adxl345-accelerometer/) <br>
- [Reference 2](https://docs.arduino.cc/language-reference/en/functions/communication/wire/requestFrom/) <br>
**Driver Completed Date** : DEC23, 2024


## BMP390 :


### Hardware Design notes :
- Connect SDO to V_dd_io so that I2C address is `0x77`.

### Software Dependancies :
- `Wire.h` library for I2C communication.


## NEO 8M GPS sensor :

Usefull links/datasheets :
1. [NEO8M Receiver Description](https://content.u-blox.com/sites/default/files/products/documents/u-blox6_ReceiverDescrProtSpec_%28GPS.G6-SW-10018%29_Public.pdf)

### General Notes 
- We want to extract GGA data from NMEA sentences.
    - i.e `$GPGGA` sentence data from NMEA packet.
    - We want the following data from `$GPGGA` sentence :
        - UTC Time
        - Latitude
        - Longitude
        - MSL Altitude 

    - For UBX protocol, this is `NAV-POSLLH` packet. 


### Software design Notes 

**UBX Packet Flow** :
- Disable all NMEA messages
- Enable UBX message (`NAV-POSLLH`) for `$GPGGA` data, i.e the data fields above. 

Structure of UBX packet : <br>

`| Syn char 1 | Syn char 2 | CLASS | ID | Length of msg (LE) | ~ payload ~ | CK_A | CK_B |`

- CK_A,B are checksum. 


### Hardware design notes :

- Use UART1 port on GPS Module.

#### Interface selection :
- If `D_SEL = 1`, then :
    - UART TX = Pin 20
    - UART RX = Pin 21 
    - DDC SCL = Pin 19
    - DDC SDA = Pin 18

