#include "stm32f4xx.h"
#include "board.h" // 提供延时函数

#define SDA_PIN GPIO_Pin_5
#define SDA_PORT GPIOB
#define SCL_PIN GPIO_Pin_9
#define SCL_PORT GPIOB

/* 基本 I2C 操作宏 */
#define SDA_HIGH() GPIO_SetBits(SDA_PORT, SDA_PIN)
#define SDA_LOW()  GPIO_ResetBits(SDA_PORT, SDA_PIN)
#define SCL_HIGH() GPIO_SetBits(SCL_PORT, SCL_PIN)
#define SCL_LOW()  GPIO_ResetBits(SCL_PORT, SCL_PIN)
#define READ_SDA() GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN)

unsigned char Ping(void);
unsigned char IIC_Get_Digtal(void);
unsigned char IIC_Get_Anolog(unsigned char * Result,unsigned char len);
unsigned char IIC_Get_Single_Anolog(unsigned char Channel);
unsigned char IIC_Anolog_Normalize(uint8_t Normalize_channel);
unsigned short IIC_Get_Offset(void );
void huidu_init(void);
