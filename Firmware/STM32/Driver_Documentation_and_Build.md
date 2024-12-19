

## Driver Development Documentation :


This document is a thought-process flow for development of drives. 
All the steps have done to develop drivers will be documented here.
The document will contain a section for the protocols I plan to use with each of the sensors.
Each sensor will have a library that I develop myself for that particular sensor using a particular protocol.

Here is the list of all sensors along side the protocols to be used with them :

| Sr. No | Sensor    | Functionality          | Protocol     |
| ------ | --------- | ---------------------- | ------------ |
| 1      | BMP390    | Pressure               | I2C          |
| 2      | ICM-20600 | Accelerometer (3-axis) | I2C          |
|        |           | Gyro (3-axis)          | I2C          |
| 3      | LoRa      | RFM95CW                | SPI          |
| 4      | GPS       | NEO6M                  | UART/SPI/I2C |



### BMP90 :

Process Flow in Libraries :

1. Initialize sensor :
    - Write intialization values to all registers from register map in Datasheet 
    - Set registers as required.
    - Custom function for intialization

2. Functions to write to BMP390 I2C address. 
3. Function to read data from correct register from BMP390

General Notes :
- Enable internal FIFO buffer of BMP390 and burst read FIFO using DMA/IRQ.



#### For pressure measurement :
- press_en bit in `PWR_CTRL[0]`
- oversamppling `osr_p` configured in `OSR[2:0]` to `000`. (1x sampling.)
- disable temperature measurement.

##### FIFO Configuration 
- `[FIFO_CONFIG_1]` register
    - `fifo_mode -> 1`, data can be collected to FIFO.
    - `fifo_stop_on_full -> 0` Deletes oldest frame in buffer and replaces with new frame
    - `fifo_press_en -> 1`, pressure data is to be collected.

- `[FIFO_CONFIG_2]` register. 
    - `data_select -> 00`. We want to store unfiltered data.  
    - `fifo_subsampling -> 000`

- `[FIFO_CONFIG_1]` -> [ R R R 0 1 0 0 1 ] 
- `[FIFO_CONFIG_2]` -> [ R R R 0 0 0 0 0 ] 
    

- Data read out using `[FIFO_DATA]` register. 


#### Values of registers :

- `PWR_CTRL` = `[R R 1 1 R R 0 1]`
- `OSR` = `[R R 0 0 0 0 0 0 ]`
- `ODR` = `[R R R 0 0 0 0 0 ]`
- `INT_CTRL` = `[ R 1 x 1 1 1 1 0 ]`
- `INT_STATUS` = `0x00`
- `FIFO_CONFIG_1` |= (fifo_press_en << 1) | (fifo_stop_on_full << 1)


### ADXL375 (High-G Acc + Gyro) :


#### Power sequencing :
- V_s and V_dd/io  = 1 1 (device in standby mode awaiting command to get measurements )
- Set D3 in `PWR_CTL` register(`0x2D`)
