#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"

/**
  * @brief  OLED初始化
  * @retval 无
  */
void OLED_Init(void);

/**
  * @brief  OLED清屏
  * @retval 无
  */
void OLED_Clear(void);

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Number, uint8_t IntLength, uint8_t FracLength);

void OLED_Printf(uint8_t Line, uint8_t Column, const char *format, ...);
#endif
