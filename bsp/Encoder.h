#ifndef _ENCODER_H
#define _ENCODER_H

#include "stm32f4xx.h"

// 函数声明
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
int Read_Speed(int TIMx);
void Encoder_TIM3_Init(void); // 改为TIM3初始化
void Encoder_TIM4_Init(void);
int Read_Velocity(int TIMx);

#endif