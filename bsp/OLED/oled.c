#include "oled.h"
#include "stm32f4xx.h"
#include "oledfont.h"
#include "board.h"          // 提供 delay_us, delay_ms
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define OLED_PRINTF_BUF_SIZE  128

/*------------------- 硬件 I2C2 引脚定义 (PB10, PB11) -------------------*/
#define I2Cx                    I2C2
#define I2Cx_SCL_PIN            GPIO_Pin_10
#define I2Cx_SDA_PIN            GPIO_Pin_11
#define I2Cx_GPIO_PORT          GPIOB
#define I2Cx_GPIO_CLK           RCC_AHB1Periph_GPIOB
#define I2Cx_CLK                RCC_APB1Periph_I2C2
#define I2Cx_SCL_SOURCE         GPIO_PinSource10
#define I2Cx_SDA_SOURCE         GPIO_PinSource11
#define I2Cx_AF                 GPIO_AF_I2C2

/* OLED I2C 地址（7位地址0x3C，左移一位后为0x78） */
#define OLED_ADDR               0x78
#define OLED_CMD_CTRL           0x00    // 控制字节：命令
#define OLED_DATA_CTRL          0x40    // 控制字节：数据

/*------------------- 硬件 I2C 初始化 -------------------*/
static void I2C_Hardware_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    /* 使能 GPIO 和 I2C 时钟 */
    RCC_AHB1PeriphClockCmd(I2Cx_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);

    /* 配置 PB10/PB11 为复用开漏模式 */
    GPIO_InitStruct.GPIO_Pin = I2Cx_SCL_PIN | I2Cx_SDA_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;   // 内部上拉
    GPIO_Init(I2Cx_GPIO_PORT, &GPIO_InitStruct);

    /* 复用功能映射 */
    GPIO_PinAFConfig(I2Cx_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_AF);
    GPIO_PinAFConfig(I2Cx_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_AF);

    /* I2C 配置：400kHz 快速模式 */
    I2C_InitStruct.I2C_ClockSpeed = 400000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2Cx, &I2C_InitStruct);
    I2C_Cmd(I2Cx, ENABLE);
}

/*------------------- 硬件 I2C 发送数据（轮询，阻塞） -------------------*/
static void I2C_Hardware_Send(uint8_t dev_addr, uint8_t ctrl_byte, const uint8_t *data, uint16_t len)
{
    uint32_t timeout = 0xFFFF;

    /* 等待总线空闲 */
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) && timeout--) __NOP();

    /* 产生起始条件 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT) && timeout--) __NOP();

    /* 发送设备地址（写方向） */
    I2C_Send7bitAddress(I2Cx, dev_addr, I2C_Direction_Transmitter);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && timeout--) __NOP();

    /* 发送控制字节（命令/数据标识） */
    I2C_SendData(I2Cx, ctrl_byte);
    timeout = 0xFFFF;
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && timeout--) __NOP();

    /* 发送数据 */
    for (uint16_t i = 0; i < len; i++)
    {
        I2C_SendData(I2Cx, data[i]);
        timeout = 0xFFFF;
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && timeout--) __NOP();
    }

    /* 产生停止条件 */
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

/*------------------- OLED 底层接口（替换原模拟 I2C 函数） -------------------*/
void OLED_WriteCommand(uint8_t Command)
{
    I2C_Hardware_Send(OLED_ADDR, OLED_CMD_CTRL, &Command, 1);
}

void OLED_WriteData(uint8_t Data)
{
    I2C_Hardware_Send(OLED_ADDR, OLED_DATA_CTRL, &Data, 1);
}

/* 批量发送数据（用于清屏等，提升速度） */
static void OLED_WriteDataBulk(const uint8_t *Data, uint16_t Len)
{
    I2C_Hardware_Send(OLED_ADDR, OLED_DATA_CTRL, Data, Len);
}

/*------------------- OLED 基础操作 -------------------*/
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);                     // 设置页地址
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));     // 设置列地址高4位
    OLED_WriteCommand(0x00 | (X & 0x0F));            // 设置列地址低4位
}

/* 优化后的清屏：每行一次性发送 128 字节 */
void OLED_Clear(void)
{
    uint8_t clear_buf[128];
    memset(clear_buf, 0, 128);   // 清空一行数据

    for (uint8_t j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        OLED_WriteDataBulk(clear_buf, 128);   // 一次发送一整行
    }
}

/*------------------- 字符显示 -------------------*/
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    uint8_t buf[16];

    /* 上半部分字模 */
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
        buf[i] = OLED_F8x16[Char - ' '][i];
    OLED_WriteDataBulk(buf, 8);

    /* 下半部分字模 */
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
        buf[i] = OLED_F8x16[Char - ' '][i + 8];
    OLED_WriteDataBulk(buf, 8);
}

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
        OLED_ShowChar(Line, Column + i, String[i]);
}

/*------------------- 数字显示辅助函数 -------------------*/
static uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) Result *= X;
    return Result;
}

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        char ch = (Number / OLED_Pow(10, Length - i - 1) % 10) + '0';
        OLED_ShowChar(Line, Column + i, ch);
    }
}

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++)
    {
        char ch = (Number1 / OLED_Pow(10, Length - i - 1) % 10) + '0';
        OLED_ShowChar(Line, Column + i + 1, ch);
    }
}

void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        else
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
    }
}

void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        char ch = (Number / OLED_Pow(2, Length - i - 1) % 2) + '0';
        OLED_ShowChar(Line, Column + i, ch);
    }
}

void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Number, uint8_t IntLength, uint8_t FracLength)
{
    uint8_t i;
    uint32_t integer, fractional;
    float temp;

    if (Number < 0)
    {
        OLED_ShowChar(Line, Column, '-');
        Number = -Number;
        Column++;
    }
    else
    {
        OLED_ShowChar(Line, Column, '+');
        Column++;
    }

    integer = (uint32_t)Number;
    OLED_ShowNum(Line, Column, integer, IntLength);
    Column += IntLength;

    OLED_ShowChar(Line, Column, '.');
    Column++;

    temp = Number - integer;
    temp += 0.5 / pow(10, FracLength);   // 四舍五入

    for (i = 0; i < FracLength; i++)
    {
        temp *= 10;
        fractional = (uint32_t)temp;
        OLED_ShowChar(Line, Column + i, (fractional % 10) + '0');
    }
}

/*------------------- OLED 初始化 -------------------*/
void OLED_Init(void)
{
    delay_ms(100);              // 等待 OLED 上电稳定
    I2C_Hardware_Init();        // 硬件 I2C 初始化（替换原模拟 I2C 初始化）

    /* OLED 初始化命令序列（标准 SSD1306） */
    OLED_WriteCommand(0xAE);   // 关闭显示

    OLED_WriteCommand(0xD5);
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8);
    OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3);
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xA1);    // 列方向正常
    OLED_WriteCommand(0xC8);    // 行方向正常
    OLED_WriteCommand(0xDA);
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81);
    OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9);
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB);
    OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0x8D);
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);    // 开启显示

    OLED_Clear();               // 清屏
}

/**
 * @brief 格式化输出到 OLED 屏幕（类似 printf）
 * @param Line    行号（1~8，对应屏幕的 8 个页）
 * @param Column  列号（1~16，每行 16 个字符）
 * @param format  格式化字符串，支持 %d, %f, %x 等标准格式
 * @param ...     可变参数
 */
void OLED_Printf(uint8_t Line, uint8_t Column, const char *format, ...)
{
    char buf[OLED_PRINTF_BUF_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    OLED_ShowString(Line, Column, buf);
}