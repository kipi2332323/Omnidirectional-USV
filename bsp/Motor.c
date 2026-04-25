#include "Motor.h"                  // Device header

void PWM_Throttle_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // 1. 使能时钟
    RCC_AHB1PeriphClockCmd(PWM_GPIO_CLK, ENABLE);        // GPIOA时钟
    RCC_APB2PeriphClockCmd(PWM_TIMER_CLK, ENABLE);       // TIM1时钟（APB2，预分频2）

    // 2. 配置GPIO为复用推挽输出（4个通道：PE9, PE11, PE13, PE14）
    GPIO_InitStruct.GPIO_Pin = PWM_PIN_GPIO_PIN_f | PWM_PIN_GPIO_PIN_b | PWM_PIN_GPIO_PIN_l | PWM_PIN_GPIO_PIN_r;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;            // 复用功能
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(PWM_PIN_GPIO_PORT_f, &GPIO_InitStruct);

    // 3. 映射GPIO到TIM1通道
    GPIO_PinAFConfig(PWM_PIN_GPIO_PORT_f, GPIO_PinSource9, GPIO_AF_TIM1);  // CH1
    GPIO_PinAFConfig(PWM_PIN_GPIO_PORT_b, GPIO_PinSource11, GPIO_AF_TIM1);  // CH2
    GPIO_PinAFConfig(PWM_PIN_GPIO_PORT_l, GPIO_PinSource13, GPIO_AF_TIM1); // CH3
    GPIO_PinAFConfig(PWM_PIN_GPIO_PORT_r, GPIO_PinSource14, GPIO_AF_TIM1); // CH4

    // 4. 配置TIM1时基：50Hz（20ms周期）
    // 时钟计算：168MHz / (168-1) = 1MHz 计数频率 → 1MHz * 20ms = 20000 计数值
    TIM_TimeBaseStruct.TIM_Prescaler = 167;              // 预分频系数：168-1
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_Period = 19999;               // 周期20000（0-19999）=20ms
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;        // 高级定时器需设0
    TIM_TimeBaseInit(PWM_TIMER, &TIM_TimeBaseStruct);

    // 5. 配置PWM输出模式（PWM1）
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStruct.TIM_Pulse = 0;               // 初始为停止状态
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    
    // 6. 初始化四个通道
    TIM_OC1Init(PWM_TIMER, &TIM_OCInitStruct);  // CH1
    TIM_OC2Init(PWM_TIMER, &TIM_OCInitStruct);  // CH2
    TIM_OC3Init(PWM_TIMER, &TIM_OCInitStruct);  // CH3
    TIM_OC4Init(PWM_TIMER, &TIM_OCInitStruct);  // CH4

    // 7. 使能预装载
    TIM_OC1PreloadConfig(PWM_TIMER, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(PWM_TIMER, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(PWM_TIMER, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(PWM_TIMER, TIM_OCPreload_Enable);
    
    TIM_ARRPreloadConfig(PWM_TIMER, ENABLE);                // 使能ARR预装载
    TIM_Cmd(PWM_TIMER, ENABLE);                             // 启动定时器
    TIM_CtrlPWMOutputs(PWM_TIMER, ENABLE);                  // 高级定时器必须开启此功能
}

// 设置单个通道PWM脉宽（范围保护）
// channel: 1/2/3/4 对应CH1/CH2/CH3/CH4（前后左右）
void Set_PWM(uint8_t channel, uint16_t value)
{
    // 范围限制：1000-2000μs
    if(value < PWM_REV_MAX) value = PWM_REV_MAX;
    if(value > PWM_FWD_MAX) value = PWM_FWD_MAX;
    
    // 根据通道号更新PWM脉宽
    switch(channel)
    {
        case 1:  // 前进通道
            TIM_SetCompare1(PWM_TIMER, value);
            break;
        case 2:  // 后退通道
            TIM_SetCompare2(PWM_TIMER, value);
            break;
        case 3:  // 左转通道
            TIM_SetCompare3(PWM_TIMER, value);
            break;
        case 4:  // 右转通道
            TIM_SetCompare4(PWM_TIMER, value);
            break;
        default:
            break;
    }
}

// 设置四个通道PWM脉宽（四合一）
// value_f: 前进、value_b: 后退、value_l: 左转、value_r: 右转
void Set_PWM_All(uint16_t value_f, uint16_t value_b, uint16_t value_l, uint16_t value_r)
{
    Set_PWM(1, value_f);  // 前进通道
    Set_PWM(2, value_b);  // 后退通道
    Set_PWM(3, value_l);  // 左转通道
    Set_PWM(4, value_r);  // 右转通道
}