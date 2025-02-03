
# Data Acquisition Computer.

This repository contains files related to development of my L1/2 certification rocket.

## Structure of Repository: 

The repo is structured as follows :
- [`Firmware`](./Firmware/) folder contains materials and code related to development of Flight Computer/DAQC, including firmware for STM32 and ESP32 platforms

    - [`Firmware/ESP32/ESP32_FC`](./Firmware/ESP32/ESP32_FC/) folder contains **working** code that will be flown on my L1 Certification rocket.
    - [`Firmware/ESP32/ESP32_DAQC`](./Firmware/ESP32/ESP32_DAQC/) folder contains experimental code. This was an attempt to write every sensor driver myself without using external libraries.
    - [`Firmware/STM32`](./Firmware/STM32/) folder contains files related to early concepts of using STM32 as a flight computer board

- `PCB` folder contains PCB related design files and documentation for STM32 and ESP32 flight computer/DAQC PCB's.
- `Structure` folder contains all files related to design of physical rocket including OpenRocket simulations and CAD files for the rocket itself.