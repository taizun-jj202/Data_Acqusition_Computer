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
 * @param[in] reg Register to which value needs to be written.
 * @param[in] value Value written to above register.
 */
void BMP390_Write_Reg( I2C_HandleTypeDef *hi2c,
                        uint8_t reg,
                        uint8_t *value)
{
    HAL_I2C_Mem_Write(hi2c, BMP390_I2C_ADDR, reg, 1, &value, 1, HAL_MAX_DELAY);
}                        


void BMP390_Read_Reg ( I2C_HandleTypeDef *hi2c, 
                        uint8_t reg, 
                        uint8_t *value
                    )
{
    HAL_I2C_Mem_Read(hi2c, BMP390_I2C_ADDR, reg, 1 , &value, 1, 1000);
}
                

/**
 * @brief Initializes registers with values for Pressure sensing only.
 * @param *hi2c Pointer to I2C_HandleTypeDef structure provided by HAL
 * @note See Driver_Documentation_and_Build.md for detailed notes on 
 *       why registers have been with particular values.
 */
void BMP390_Init (
                    I2C_HandleTypeDef *hi2c
                )
{
    // Power mode : Normal (D5 & D4 = 1 1) and press_en = 1
    BMP390_Write_Reg(hi2c, BMP390_PWR_CTRL_REG, (BMP390_PWR_CTRL | (1<<4) | (1<<5) | (0<<1)) ); 

    // OSR[2:0] -> 100 (X16 oversampling), 
    BMP390_Write_Reg(hi2c, BMP390_OSR_REG , (BMP390_OSR | 0x04 ));

    // `INT_CTRL` = `[ R 1 x 1 1 1 1 0 ]` -> 0x01011110
    BMP390_Write_Reg(hi2c, BMP390_INT_CTRL_REG, (BMP390_INT_CTRL | 0x5E));

    //FIFO Watermark registers :
    BMP390_Write_Reg(hi2c, BMP390_FIFO_WTM_1_REG, (BMP390_FIFO_WTM_1 | 0x01));
    BMP390_Write_Reg(hi2c, BMP390_FIFO_WTM_0_REG, (BMP390_FIFO_WTM_0 | 0xE0));

}


