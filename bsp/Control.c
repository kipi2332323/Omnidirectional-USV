#include "Control.h"
#include "Sys.h"
#include "oled.h"

void TIM7_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 1. 使能定时器7时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    
    // 2. 配置定时器基础参数
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;        // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 84 -1;       // 预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    
    // 3. 使能定时器更新中断
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    
    // 4. 配置NVIC中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 5. 使能定时器
    TIM_Cmd(TIM7, ENABLE);
}


