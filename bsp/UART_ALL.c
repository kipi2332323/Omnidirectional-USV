#include "UART_ALL.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <board.h>
#include <stdlib.h>
#include "fifo.h"

void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 使能 GPIOA 和 USART1 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 2. 配置 GPIO 复用功能
    // TX 引脚：PA9
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

    // RX 引脚：PA10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    // 3. 配置 UART 参数
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStruct);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); // 使能空闲中断，用于帧接收完成判断

    // 4. 配置 USART1 中断优先级
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 5. 使能 USART1
    USART_Cmd(USART1, ENABLE);
}

void USART2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 使能 GPIOD 和 USART2 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // 2. 配置 GPIO 复用功能
    // TX 引脚：PD5
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);

    // RX 引脚：PD6
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    // 3. 配置 UART 参数
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART2, &USART_InitStruct);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_IDLE, DISABLE); // USART2 仅使用接收中断

    // 4. 配置 USART2 中断优先级
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 5. 使能 USART2
    USART_Cmd(USART2, ENABLE);
}

void USART3_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    // 1. 使能 USART3 和 GPIOD 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // 2. 配置 GPIO
    // TX 引脚：PD8
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    // RX 引脚：PD9
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    // 3. 配置 GPIO 复用到 USART3
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

    // 4. 配置 USART 参数
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStruct);

    // 5. 预留 USART3 中断配置，目前默认关闭
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStruct);

    /* 默认不启用 USART3 接收和空闲中断 */
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART3, USART_IT_IDLE, DISABLE);

    // 6. 使能 USART3
    USART_Cmd(USART3, ENABLE);
}

void UART4_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    // 1. 使能 UART4 和 GPIOC 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    // 2. 配置 GPIO
    // TX 引脚：PC10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // RX 引脚：PC11
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // 3. 配置 GPIO 复用到 UART4
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

    // 4. 配置 UART 参数
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART4, &USART_InitStruct);

    // 5. 配置 UART4 中断优先级
    NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 6. 使能 UART4
    USART_Cmd(UART4, ENABLE);
}

void UART5_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 使能 GPIOC、GPIOD 和 UART5 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

    // 2. 配置 GPIO 复用功能
    // TX 引脚：PC12
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

    // RX 引脚：PD2
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

    // 3. 配置 UART 参数
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(UART5, &USART_InitStruct);

    // 4. 使能 UART5 接收中断和空闲中断
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);

    // 5. 配置 UART5 中断优先级
    NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 6. 使能 UART5
    USART_Cmd(UART5, ENABLE);
}

void UART_ALL_Init(void)
{
    USART1_Init();
    USART2_Init();
//  USART3_Init();
//  UART4_Init();
    UART5_Init();
}

////////////// 串口通用函数封装 //////////////

static USART_TypeDef *usart_for_printf = 0; // 记录当前用于 printf 重定向的串口

//
// @简介：通过串口发送 1 个字节
//
// @参数 USARTx：串口实例，如 USART1、USART2、USART3
// @参数 Data  ：待发送的数据
//
void My_USART_SendByte(USART_TypeDef *USARTx, const uint8_t Data)
{
    My_USART_SendBytes(USARTx, &Data, 1);
}

//
// @简介：通过串口发送多个字节
//
// @参数 USARTx：串口实例，如 USART1、USART2、USART3
// @参数 pData ：待发送的数据缓冲区
// @参数 Size  ：待发送字节数
//
void My_USART_SendBytes(USART_TypeDef *USARTx, const uint8_t *pData, uint16_t Size)
{
    if(Size == 0) return;

    for(uint16_t i = 0; i < Size; i++)
    {
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

        USART_SendData(USARTx, pData[i]);
    }

    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

//
// @简介：通过串口发送 1 个字符
//
// @参数 USARTx：串口实例
// @参数 C     ：待发送字符
//
void My_USART_SendChar(USART_TypeDef *USARTx, const char C)
{
    My_USART_SendBytes(USARTx, (const uint8_t *)&C, 1);
}

//
// @简介：通过串口发送字符串
//
// @参数 USARTx：串口实例
// @参数 Str   ：待发送字符串
//
void My_USART_SendString(USART_TypeDef *USARTx, const char *Str)
{
    My_USART_SendBytes(USARTx, (const uint8_t *)Str, strlen(Str));
}

//
// @简介：通过串口执行格式化输出
//
// @参数 USARTx：串口实例
// @参数 Format：格式化字符串
// @参数 ...   ：可变参数
//
// My_USART_Printf(UART4, "%d", 5);
void My_USART_Printf(USART_TypeDef *USARTx, const char *Format, ...)
{
    usart_for_printf = USARTx;
    va_list args;
    va_start(args, Format);
    vprintf(Format, args);
    va_end(args);
}

//
// @简介：从串口读取 1 个字节
//
// @参数 USARTx：串口实例
// @返回值：读取到的字节
//
uint8_t My_USART_ReceiveByte(USART_TypeDef *USARTx)
{
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);

    return USART_ReceiveData(USARTx);
}

//
// @简介：重写 fputc，实现 printf 输出到指定串口
//
int fputc(int ch, FILE *f)
{
    // 1. 等待发送数据寄存器空
    while(USART_GetFlagStatus(usart_for_printf, USART_FLAG_TXE) == RESET);
    // 2. 写入待发送字节
    USART_SendData(usart_for_printf, (uint8_t)ch);

    return ch;
}

///////////////////////////////////////// USART1 接收中断 /////////////////////////////////////////
__IO uint8_t USART1_RX[128] = {0};
__IO uint8_t USART1_RX_Count = 0;

/**
 * @brief USART1 中断服务函数
 */
void USART1_IRQHandler(void)
{
/**********************************************************
*** 串口接收中断
**********************************************************/
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        // 将接收到的数据存入缓冲区
        if(USART1_RX_Count < 128)
        {
            USART1_RX[USART1_RX_Count++] = (uint8_t)USART1->DR;
        }
        // 清除接收中断标志
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

/**********************************************************
*** 串口空闲中断，表示一帧数据接收完成
**********************************************************/
    else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        // 先读 SR 再读 DR，清除 IDLE 中断
        USART1->SR; USART1->DR;
        // 数据已存入 USART1_RX，USART1_RX_Count 为当前帧长度
        // 上层处理完成后需手动清零 USART1_RX_Count
    }
}

/* GY25T 陀螺仪发送指令 */
const uint8_t tuoluoyi_correct[5] = {0x00,0x06,0x06,0x10,0x1c};
const uint8_t tuoluoyi_clear[5] = {0x00,0x06,0x06,0x01,0x0d};
const uint8_t gy25t_stream_gyro_z_yaw[5] = {0xA4,0x03,0x12,0x08,0xC1};
const uint8_t gy25t_stream_continuous[5] = {0xA4,0x06,0x03,0x00,0xAD};

///////////////////////////////////////// USART2 接收中断 /////////////////////////////////////////
__IO uint8_t USART2_RX[128] = {0};
__IO uint8_t USART2_RX_Count = 0;
int16_t gyro_z_raw = 0;
float gyro_z_dps = 0;
int16_t yaw_angle_raw = 0; // 原始偏航角数据
float yaw_angle_real = 0;

static float GY25T_RawToDps(int16_t raw)
{
    return raw * 2000.0f / 32768.0f;
}

void GY25T_ConfigStream(void)
{
    My_USART_SendBytes(USART2, gy25t_stream_gyro_z_yaw, 5);
    delay_ms(10);
    My_USART_SendBytes(USART2, gy25t_stream_continuous, 5);
}

void USART2_IRQHandler(void)
{
    static uint8_t frame_buf[13] = {0};
    static uint8_t frame_idx = 0;
    uint8_t rx = 0;
    uint8_t i = 0;

    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        rx = (uint8_t)USART_ReceiveData(USART2);

        if(frame_idx == 0)
        {
            if(rx == 0xA4)
            {
                frame_buf[frame_idx++] = rx;
            }
        }
        else
        {
            frame_buf[frame_idx++] = rx;

            if(frame_idx >= 13)
            {
                if(frame_buf[0] == 0xA4)
                {
                    for(i = 0; i < 13; i++)
                    {
                        USART2_RX[i] = frame_buf[i];
                    }
                    USART2_RX_Count = 13;

                    if(frame_buf[2] == 0x12 && frame_buf[3] == 0x08)
                    {
                        gyro_z_raw = (int16_t)(((uint16_t)frame_buf[4] << 8) | frame_buf[5]);
                        gyro_z_dps = GY25T_RawToDps(gyro_z_raw);
                        yaw_angle_raw = (int16_t)(((uint16_t)frame_buf[10] << 8) | frame_buf[11]);
                        yaw_angle_real = yaw_angle_raw / 100.0f;
                    }
                    else if(frame_buf[2] == 0x14 && frame_buf[3] == 0x08)
                    {
                        yaw_angle_raw = (int16_t)(((uint16_t)frame_buf[8] << 8) | frame_buf[9]);
                        yaw_angle_real = yaw_angle_raw / 100.0f;
                    }
                }

                // 无论帧是否有效，都复位索引，避免越界和脏数据残留
                frame_idx = 0;
            }
        }

        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

void USART3_IRQHandler(void)
{
    // USART3 接收中断当前未启用，保留空函数避免误用
}

///////////////////////////////////////// UART5 接收中断 /////////////////////////////////////////
__IO uint8_t UART5_RX[128] = {0};
__IO uint8_t UART5_RX_Count = 0;

// UART5 数据处理函数，在中断中被调用
__weak void UART5_DataProcess(uint8_t *data)
{
    if(data[0] == 0xA5 && data[10] == 0x5A)
    {
        Self_left  = data[1] & 0x01;
        Self_right = (data[1] >> 1) & 0x01;
        Move_Mode_flag  = (data[1] >> 2) & 0x01;
        Motor_switch =  (data[1] >> 3) & 0x01;
        x = (int16_t)(((uint16_t)data[6] << 8) | data[5]);
        y = (int16_t)(((uint16_t)data[8] << 8) | data[7]);
        Kp = (float)((int8_t)data[2] + 127) * 8 / 255;
        Ki = (float)((int8_t)data[3] + 127) * 8 / 255;
        Kd = (float)((int8_t)data[4] + 127) * 8 / 255;
    }
}

void UART5_IRQHandler(void)
{
/**********************************************************
*** UART5 接收中断
**********************************************************/
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        // 将接收到的数据存入缓冲区
        if(UART5_RX_Count < 128)
        {
            UART5_RX[UART5_RX_Count++] = (uint8_t)UART5->DR;
        }
        // 清除接收中断标志
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }

/**********************************************************
*** UART5 空闲中断，表示一帧数据接收完成，并在中断中直接处理
**********************************************************/
    else if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
    {
        // 先读 SR 再读 DR，清除 IDLE 中断
        UART5->SR; UART5->DR;

        // 在中断中直接处理一帧数据
        if(UART5_RX_Count > 0)
        {
            UART5_DataProcess(UART5_RX); // 调用数据处理函数
            UART5_RX_Count = 0;          // 清空计数，准备接收下一帧
        }
    }
}
