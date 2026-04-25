#include "Encoder.h"




/*********************************************************
 * 功能说明：初始化TIM3作为正交编码器接口
 * 引脚配置：
 *   TIM3_CH1 -> PA6 (A相)
 *   TIM3_CH2 -> PA7 (B相)
 * 注意事项：
 *   1. 使用TIM3替代原来的TIM2
 *   2. 保持相同的逻辑结构和配置参数
 *********************************************************/
void Encoder_TIM3_Init(void)
{
    /* 开启GPIOA和TIM3时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // F4使用AHB1总线
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* 配置PA6和PA7为复用功能模式 */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;        // 复用模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;  // F4支持更高速度
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;    // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* 配置引脚复用功能 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3); // PA6 -> TIM3_CH1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3); // PA7 -> TIM3_CH2

    /* 配置定时器时基单元 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 65535;        // 最大计数值
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0;          // 无预分频
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    /* 配置编码器接口模式 */
    TIM_EncoderInterfaceConfig(
        TIM3,                       // 使用TIM3
        TIM_EncoderMode_TI12,       // TI1和TI2双通道计数
        TIM_ICPolarity_Rising,      // 通道1上升沿有效
        TIM_ICPolarity_Rising       // 通道2上升沿有效
    );

    /* 配置输入捕获参数（滤波） */
    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 10;  // 10个时钟周期的输入滤波
    TIM_ICInit(TIM3, &TIM_ICInitStruct); // 应用配置到两个通道

    /* 启动定时器 */
    TIM_Cmd(TIM3, ENABLE);
}

/*********************************************************
 * 功能说明：初始化TIM4作为正交编码器接口
 * 引脚配置：
 *   TIM4_CH1 -> PD12 (A相)
 *   TIM4_CH2 -> PD13 (B相)
 * 注意事项：
 *   1. 保持TIM4配置，但更换引脚
 *   2. 适配F4系列GPIO配置
 *********************************************************/
void Encoder_TIM4_Init(void)
{
    /* 开启GPIOD和TIM4时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* 配置PD12和PD13为复用功能模式 */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* 配置引脚复用功能 */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); // PD12 -> TIM4_CH1
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); // PD13 -> TIM4_CH2

    /* 配置定时器基础参数 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 65535;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

    /* 配置编码器接口模式 */
    TIM_EncoderInterfaceConfig(
        TIM4, 
        TIM_EncoderMode_TI12,
        TIM_ICPolarity_Rising,
        TIM_ICPolarity_Rising
    );

    /* 配置输入捕获参数 */
    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 10;  // 保持相同滤波设置
    TIM_ICInit(TIM4, &TIM_ICInitStruct);

    /* 启动定时器 */
    TIM_Cmd(TIM4, ENABLE);
}

/*********************************************************
 * 功能说明：读取编码器速度值
 * 参数说明：
 *   TIMx - 定时器编号（3或4）
 * 返回值：
 *   编码器计数值（带符号的脉冲数）
 *********************************************************/
int Read_Speed(int TIMx)
{
    int value_1;
    switch(TIMx)
    {
        case 3:  
            value_1 = (short)TIM_GetCounter(TIM3);
            TIM_SetCounter(TIM3, 0);
            break;
        case 4:  
            value_1 = (short)TIM_GetCounter(TIM4);
            TIM_SetCounter(TIM4, 0);
            break;
        default:
            value_1 = 0;
    }
    return value_1;
}

/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Velocity(int TIMx)
{
    int Encoder_TIM;    
   switch(TIMx)
	 {
		case 2:  Encoder_TIM= (short)TIM2 -> CNT; 	TIM2->CNT=0; break;
		case 3:  Encoder_TIM= (short)TIM3 -> CNT; 	TIM3->CNT=0; break;	
		case 4:  Encoder_TIM= (short)TIM4 -> CNT;  	TIM4->CNT=0; break;	
		default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

/*********************************************************
 * TIM3中断服务函数
 * 功能：处理计数器溢出
 * 注意：F4系列中断处理函数名与F1不同
 *********************************************************/
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
				
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        /* 可在此添加溢出计数逻辑 */
    }
}

/*********************************************************
 * TIM4中断服务函数
 * 注意：F4系列中断处理函数名与F1相同
 *********************************************************/
void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}