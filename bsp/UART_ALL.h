 #ifndef UART1_H_
 #define UART1_H_
 
#include "stm32f4xx.h"
#include <stdio.h>
#include "fifo.h"
#include <stdbool.h>
#include "Sys.h"

extern __IO uint8_t USART1_RX[128];
extern __IO uint8_t USART1_RX_Count;
extern __IO uint8_t USART2_RX[128];
extern __IO uint8_t USART2_RX_Count;
extern __IO uint8_t UART5_RX[128];
extern __IO uint8_t UART5_RX_Count;
extern int16_t gyro_z_raw;
extern float gyro_z_dps;
extern int16_t yaw_angle_raw;
extern float yaw_angle_real; // 当前偏航角，单位：度

/* GY25T 陀螺仪发送指令 */
extern const uint8_t tuoluoyi_correct[5];
extern const uint8_t tuoluoyi_clear[5];
extern const uint8_t gy25t_stream_gyro_z_yaw[5];
extern const uint8_t gy25t_stream_continuous[5];

void USART1_Init(void);
void USART2_Init(void);
void USART3_Init(void);
void UART4_Init(void);
void UART5_Init(void);
void UART_ALL_Init(void);

void My_USART_SendByte(USART_TypeDef *USARTx, const uint8_t Data);
void My_USART_SendBytes(USART_TypeDef *USARTx, const uint8_t *pData, uint16_t Size);
void My_USART_SendChar(USART_TypeDef *USARTx, const char C);
void My_USART_SendString(USART_TypeDef *USARTx, const char *Str);
void My_USART_Printf(USART_TypeDef *USARTx, const char *Format, ...);
void UART5_DataProcess(uint8_t *data);
void GY25T_ConfigStream(void);

#endif
