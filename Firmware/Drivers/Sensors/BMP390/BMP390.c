/**
 * @author Taizun Jafri
 * @date 14th Dec 2024'
 * @brief Definition of functions required for I2C comm for BMP390 sensor
 */


#include "stm32g0xx_hal.h"
#include <stdint.h>
#include "BMP390.h"



/** 
 * @brief Function to write to registers in BMP390
 * @param *hi2c Pointer to I2C_HandleTypeDef structure provided by HAL
 * @param reg Register to which value needs to be written.
 * @param value Value written to above register.
 */
void BMP390_Write_Reg( I2C_HandleTypeDef *hi2c,
                        uint8_t reg,
                        uint8_t value)
{
    HAL_I2C_Mem_Write(hi2c, BMP390_I2C_ADDR, reg, 1, &value, 1, HAL_MAX_DELAY);
}                        


void BMP390_Read_Reg ( I2C_HandleTypeDef *hi2c, 
                        uint8_t reg, 
                        uint8_t value
                    )
{
    HAL_I2C_Mem_Read(hi2c, BMP390_I2C_ADDR, reg, 1 , &value, 1, 1000);
}
                

void BMP390_Init (
                    I2C_HandleTypeDef *hi2c
                )
{
    
}