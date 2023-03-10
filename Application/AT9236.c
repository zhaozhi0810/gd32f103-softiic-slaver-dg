/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：AT9236.c
* 摘要：AT9236驱动
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/

#include "AT9236.h"

#define I2C_AT9236         I2C1
#define I2C1_AT9236_ADDR   0x78

/*设置PWM亮度*/
void AT9236_PWM_control(uint8_t pwm)
{
    int i = 0;
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, 0x01);
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    for (i = 0; i < 36; i++)
    {
        i2c_data_transmit(I2C_AT9236, pwm);
        while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
}

/*点亮全部LED灯*/
void AT9236_LED_lightAll(void)
{
    int i = 0;
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, 0x26);
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    for (i = 0; i < 36; i++)   //多功能型号是36个按键，其他型号是33个按键。
    {
        i2c_data_transmit(I2C_AT9236, 0x01);
        while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
}

/*关闭全部LED灯*/
void AT9236_LED_lightAll_OFF(void)
{
    int i = 0;
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, 0x26);
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    for (i = 0; i < 36; i++)   //多功能型号是36个按键，其他型号是33个按键。
    {
        i2c_data_transmit(I2C_AT9236, 0x00);
        while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
}

/*LED灯单独控制*/
void AT9236_LED_control(uint8_t write_address, LED_control_t cmd)
{
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, write_address);  
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, cmd);
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
}

/*IIC发送数据至AT9236*/
void AT9236_transmit_byte(uint8_t write_address, uint8_t cmd)
{
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C_AT9236, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C_AT9236);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C_AT9236, I2C1_AT9236_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set*/
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C_AT9236, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (SET != i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, write_address);
    /* wait until BTC bit is set */
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    i2c_data_transmit(I2C_AT9236, (uint8_t)cmd);
    while (!i2c_flag_get(I2C_AT9236, I2C_FLAG_TBE));
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C_AT9236);
    /* wait until the stop condition is finished */
    while (I2C_CTL0(I2C_AT9236) & 0x0200);
}
