#include "stm32f4xx.h"                  // Device header
#include "Sys.h" 


// ï¿½ï¿½ï¿½ï¿½PWMï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å£ï¿½PA8ï¿½ï¿½TIM1_CH1ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½F407ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í¨ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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

// AM32ï¿½ï¿½ï¿½PWMï¿½ï¿½ï¿½å£¨ï¿½ï¿½ï¿½ï¿½ï¿½Þ¸Ä£ï¿½
#define PWM_STOP     1500    // ï¿½ï¿½Î»ï¿½ï¿½Í£Ö¹
#define PWM_REV_MAX  1000    // ï¿½ï¿½×ªï¿½ï¿½ï¿½Öµï¿½ï¿?000ï¿½ï¿½sï¿½ï¿½
#define PWM_FWD_MAX  2000    // ï¿½ï¿½×ªï¿½ï¿½ï¿½Öµï¿½ï¿?000ï¿½ï¿½sï¿½ï¿½
#define PWM_STEP     5       // ï¿½ï¿½ï¿½Ù²ï¿½ï¿½ï¿½
#define DELAY_STEP   20      // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½msï¿½ï¿½
#define DELAY_HOLD   3000    // Ã¿ï¿½ï¿½×´Ì¬ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ä£¨msï¿½ï¿½


// º¯ÊýÉùÃ÷
void PWM_Throttle_Init(void);
void Set_PWM(uint8_t channel, uint16_t value);
void Set_PWM_All(uint16_t value_f, uint16_t value_b, uint16_t value_l, uint16_t value_r);

