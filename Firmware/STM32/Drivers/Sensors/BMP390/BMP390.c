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

    /**
     * @brief Elaborates on bits set in register.
     * `INT_CTRL` = `[ R 1 x 1 1 1 1 0 ]` -> 0x01011110
     *  drdry = 1 
     *  ffull_en = 1
     *  fwtm_en  = 1
     *  int_latch = 1
     *  int_level = 1  
     * */ 
    BMP390_Write_Reg(hi2c, BMP390_INT_CTRL_REG, (BMP390_INT_CTRL | 0x5E));


    /** 
     * [FIFO_CONFIG_1] : 
     * fifo_mode = 1 (enable FIFO)
     * fifo_stop_on_full = 1
     * fifo_press_en = 1
     */
    BMP390_Write_Reg(hi2c, BMP390_FIFO_CONFIG_1_REG, ( BMP390_FIFO_CONFIG_1 | (0 << 1) |
                                                                              (1 << 1) |
                                                                              (3 << 1) ) );

    // FIFO Watermark registers :
    BMP390_Write_Reg(hi2c, BMP390_FIFO_WTM_1_REG, (BMP390_FIFO_WTM_1 | 0x01));
    BMP390_Write_Reg(hi2c, BMP390_FIFO_WTM_0_REG, (BMP390_FIFO_WTM_0 | 0xE0));
    BMP390_Write_Reg(hi2c, BMP390_CMD_REG, (BMP390_CMD | 0xB0)); // Command to soft reset FIFO.

}



/**
 * @brief Reads 512B FIFO
 * @param[in] *pressure_buffer External buffer to where FIFO is extracted. 
 * @param[in] *num_samples Requied to convert raw pressure value to pressure in Pascal.
 * @note external buffer should be defined as  :
 *       float buffer[170]; and 
 *       uint16_t num_samples = 0 
 */
void BMP390_ReadFIFO ( I2C_HandleTypeDef *hi2c, 
                        float *pressure_buffer,
                        uint16_t *num_samples)
{
    uint8_t length_bytes[2];
    uint16_t fifo_length;
    uint8_t fifo_data[512]; 

    fifo_length = (((uint16_t)length_bytes[1] << 8) | length_bytes[0] );

    *num_samples = fifo_length / 3;
    for( uint16_t i = 0; i < *num_samples; i ++ )  {
        uint32_t raw_pressure = ((fifo_data[i * 3 + 2] << 16) |  (fifo_data[i * 3 + 1] << 8) | fifo_data[i * 3]);
        pressure_buffer[i] = (float) raw_pressure / 256;
    }

}

