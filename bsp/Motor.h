#include "stm32f4xx.h"                  // Device header
#include "Sys.h"

// TIM1 的 4 路 PWM 输出分别对应前、后、左、右推进器
#define PWM_PIN_GPIO_PORT_f GPIOE
#define PWM_PIN_GPIO_PORT_b GPIOE
#define PWM_PIN_GPIO_PORT_l GPIOE
#define PWM_PIN_GPIO_PORT_r GPIOE
#define PWM_PIN_GPIO_PIN_f  GPIO_Pin_9   // TIM1_CH1
#define PWM_PIN_GPIO_PIN_b  GPIO_Pin_11  // TIM1_CH2
#define PWM_PIN_GPIO_PIN_l  GPIO_Pin_13  // TIM1_CH3
#define PWM_PIN_GPIO_PIN_r  GPIO_Pin_14  // TIM1_CH4
#define PWM_TIMER         TIM1
#define PWM_TIMER_CLK     RCC_APB2Periph_TIM1
#define PWM_GPIO_CLK      RCC_AHB1Periph_GPIOE

// AM32 电调 PWM 输出范围
#define PWM_STOP     1500    // 中位，表示停止
#define PWM_REV_MAX  1000    // 反转最大值，对应 1000 us
#define PWM_FWD_MAX  2000    // 正转最大值，对应 2000 us
#define PWM_STEP     5       // 调速步进
#define DELAY_STEP   20      // 调速步进间隔，单位 ms
#define DELAY_HOLD   3000    // 每种状态保持时间，单位 ms

// 函数声明
void PWM_Throttle_Init(void);
void Set_PWM(uint8_t channel, uint16_t value);
void Set_PWM_All(uint16_t value_f, uint16_t value_b, uint16_t value_l, uint16_t value_r);
