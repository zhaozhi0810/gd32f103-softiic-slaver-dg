/*
* Copyright@2022-2022，湖南航天捷诚电子装备有限责任公司
* All rights reserved.
* 文件名称：iic.c
* 摘要：自定义iic配置
* 当前版本：1.0，蒋润东、20220628
* 历史版本：无
*/
#include "iic.h"
#include "soft_iic_master_app.h"
#include "software_iic_slave.h"


#define I2C0_SLAVE_ADDRESS7     0x80
#define I2C1_SLAVE_ADDRESS7     0xAC
#define I2C0_SPEED              100000
#define I2C1_SPEED              100000



#if 0
void IIc_dey(short us)
{
    int i, j;
    for (i = 0; i < us; i++)
        for (j = 0; j < 10; j++);
}

/*初始化IIC0*/
void CH453_IIC_Init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure led GPIO port */
    gpio_init(CH453_IO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CH453_SCL_IO_PIN);
    gpio_init(CH453_IO_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, CH453_SDA_IO_PIN);
    GPIO_BC(CH453_IO_PORT) = CH453_SCL_IO_PIN | CH453_SDA_IO_PIN;
    IIC_SCL_SET;
    IIC_SDA_SET;
}

/*产生IIC起始信号*/
void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
    IIC_SDA_SET;
    IIC_SCL_SET;
    IIc_dey(2);
    IIC_SDA_RESET;//START:when CLK is high,DATA change form high to low
    IIc_dey(2);
    IIC_SCL_RESET;//钳住I2C总线，准备发送或接收数据
}

/*产生IIC停止信号*/
void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    IIC_SCL_RESET;
    IIC_SDA_RESET;//STOP:when CLK is high DATA change form low to high
    IIc_dey(2);
    IIC_SCL_SET;
    IIc_dey(2);
    IIC_SDA_SET;//发送I2C总线结束信号
    IIc_dey(2);
}

/*
等待应答信号到来
返回值：1，接收应答失败
        0，接收应答成功
*/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN();      //SDA设置为输入
    IIC_SDA_SET;
    IIc_dey(4);//delay_us(1);
    IIC_SCL_SET;
    IIc_dey(4);//delay_us(1);
    while (READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_RESET;//时钟输出0
    return 0;
}

/*产生ACK应答*/
void IIC_Ack(void)
{
    IIC_SCL_RESET;
    SDA_OUT();
    IIC_SDA_RESET;
    IIc_dey(2);//delay_us(2);
    IIC_SCL_SET;
    IIc_dey(2);//delay_us(2);
    IIC_SCL_RESET;
}

/*不产生ACK应答*/
void IIC_NAck(void)
{
    IIC_SCL_RESET;
    SDA_OUT();
    IIC_SDA_SET;
    IIc_dey(2);//delay_us(2);
    IIC_SCL_SET;
    IIc_dey(2);//delay_us(2);
    IIC_SCL_RESET;
}

/*
IIC发送一个字节
返回从机有无应答
1，有应答
0，无应答
*/
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL_RESET;//拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        uint8_t i = (txd & 0x80) >> 7;
        if (i)
        {
            IIC_SDA_SET;
        }
        else
        {
            IIC_SDA_RESET;
        }
        txd <<= 1;
        IIc_dey(2);
        IIC_SCL_SET;
        IIc_dey(2);
        IIC_SCL_RESET;
    }
    SDA_IN();      //SDA设置为输入
    IIc_dey(2);
    IIC_SCL_SET;
    IIc_dey(2);
    IIC_SCL_RESET;
}

/*读1个字节，ack=1时，发送ACK，ack=0，发送nACK */
uint8_t IIC_Read_Byte()
{
    unsigned char i, receive = 0;
    SDA_IN();//SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        IIC_SCL_RESET;
        IIc_dey(2);
        IIC_SCL_SET;
        receive <<= 1;
        if (READ_SDA) receive++;
        IIc_dey(2);
    }
    return receive;
}

/*写命令*/
void CH453_Write(uint16_t cmd)
{
    IIC_Start();
    IIC_Send_Byte((uint8_t)(cmd >> 8));
    IIC_Send_Byte((uint8_t)cmd);         /*发送数据*/
    IIC_Stop();
}

#endif


/*!
    \brief      cofigure the GPIO ports.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC0_gpio_config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config(GPIO_I2C0_REMAP, ENABLE);
    /* I2C0 GPIO ports */
    /* connect PB8 to I2C0_SCL */
    /* connect PB9 to I2C0_SDA */
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9);
}

/*!
    \brief      cofigure the I2C0 and I2C1 interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC0_config(void)
{
    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C0);
    /* I2C clock configure */
    i2c_clock_config(I2C0, I2C0_SPEED, I2C_DTCY_2);
    /* I2C address configure */
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_SLAVE_ADDRESS7);
    /* enable I2C0 */
    i2c_enable(I2C0);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}


/*!
    \brief      cofigure the GPIO ports.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC1_gpio_config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* I2C1 GPIO ports */
    /* connect PB10 to I2C1_SCL */
    /* connect PB11 to I2C1_SDA */
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);
}

/*!
    \brief      cofigure the NVIC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC0_IRQ_config(void)
{
    nvic_irq_enable(I2C0_EV_IRQn, 6, 1);
    nvic_irq_enable(I2C0_ER_IRQn, 6, 1);
    /* enable the I2C0 interrupt */
    i2c_interrupt_enable(I2C0, I2C_INT_ERR);
    i2c_interrupt_enable(I2C0, I2C_INT_EV);
    i2c_interrupt_enable(I2C0, I2C_INT_BUF);
}

/*!
    \brief      cofigure the I2C0 and I2C1 interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC1_config(void)
{
    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C1);
    /* configure I2C1 clock */
    i2c_clock_config(I2C1, I2C1_SPEED, I2C_DTCY_2);
    /* configure I2C1 address */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_SLAVE_ADDRESS7);
    /* enable I2C1 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
}

/*!
    \brief      cofigure the NVIC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void IIC1_IRQ_config(void)
{
    nvic_irq_enable(I2C1_EV_IRQn, 0, 1);
    nvic_irq_enable(I2C1_ER_IRQn, 0, 1);
    /* enable the I2C1 interrupt */
    i2c_interrupt_enable(I2C1, I2C_INT_ERR);
    i2c_interrupt_enable(I2C1, I2C_INT_EV);
    i2c_interrupt_enable(I2C1, I2C_INT_BUF);
}

/*iic初始化*/
void IIC_init(void)
{	
//    IIC0_gpio_config();
//    IIC0_config();
//    IIC0_IRQ_config();
//	InitSwSlaveI2C0();
//    IIC1_gpio_config();
//    IIC1_config();
//	IicApp_Init(IIC2_INDEX);
}

