/**
 * @author Taizun Jafri
 * @file This file is for interfacing the Bosch BMP390 Pressure
 *
 */

#ifndef BMP390_H
#define BMP390_H

#include "stm32g0xx_hal.h"
#include <stdint.h>

// Register Addresses
#define BMP390_CHIP_ID_REG  0x00
#define BMP390_PWR_CTRL_REG 0x1B
#define BMP390_OSR_REG 0x1C
#define BMP390_ODR_REG 0x0D
#define BMP390_INT_CTRL_REG 0x19
#define BMP390_FIFO_CONFIG_1_REG 0x17
#define BMP390_FIFO_CONFIG_2_REG 0x18
#define BMP390_FIFO_WTM_1_REG 0x16 
#define BMP390_FIFO_WTM_0_REG 0x15
#define BMP390_FIFO_DATA_REG 0x14
#define BMP390_FIFO_FILL_LENGTH_1_REG 0x13
#define BMP390_FIFO_FILL_LENGTH_0_REG 0x12
#define BMP390_INT_STATUS_REG 0x11
#define BMP390_CMD_REG 0x7E

//Default register values.
#define BMP390_PWR_CTRL 0x00
#define BMP390_OSR 0x02
#define BMP390_ODR 0X00
#define BMP390_INT_CTRL 0x02
#define BMP390_FIFO_CONFIG_2 0x02
#define BMP390_FIFO_CONFIG_1 0x02
#define BMP390_FIFO_WTM_1 0x00
#define BMP390_FIFO_WTM_0 0x01
#define BMP390_FIFO_FILL_LENGTH_1 0x00
#define BMP390_FIFO_FILL_LENGTH_0 0x00
#define BMP390_FIFO_DATA 0x00
#define BMP390_INT_STATUS 0x00
#define BMP390_CMD 0x00



// Alias for bits in registers 
#define fifo_press_en 3
#define fifo_stop_on_full 1

// BMP390 I2C Address :
/**
 * Connect SDO pin to GND for ADDR = 0x76
 *      else SDO high will give ADDR = 0x77
 */
#define BMP390_I2C_ADDR (0X76 << 1)

#define BMP390_FIFO_WATERMARK_LEVEL 480 // 480B filled in FIFO will trigger FIFO_WTM_INT



void BMP390_Init(I2C_HandleTypeDef *hi2c);

void BMP390_Write_Reg (I2C_HandleTypeDef *hi2c, 
                        uint8_t reg,
                        uint8_t *value);

void BMP390_Read_Reg (I2C_HandleTypeDef *hi2c,
                        uint8_t reg,
                        uint8_t *value);

void BMP390_ReadFIFO (I2C_HandleTypeDef *h2ic, 
                        float *pressure_buffer,
                        uint16_t *num_samples);
                        


#endif /*BMP390_H*/