#include "stm32f4xx.h"

// 初始化ADC1，配置PA4/5/6/7为模拟输入
void Adc_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    ADC_InitTypeDef ADC_InitStruct;

    // 使能GPIOA时钟和ADC1时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // 配置PA4、PA5、PA6、PA7为模拟输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;      // 模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  // 无上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ADC复位（可选，将ADC1全部寄存器恢复默认）
    ADC_DeInit();

    // ADC公共配置
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;   // 84MHz / 4 = 21MHz
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);

    // ADC1单独配置
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;   // 单次转换模式
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // 软件触发
    ADC_InitStruct.ADC_NbrOfConversion = 1;            // 规则组通道数1（每次只配置一个）
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;         // 关闭扫描模式（单通道）
    ADC_Init(ADC1, &ADC_InitStruct);

    // 使能ADC1
    ADC_Cmd(ADC1, ENABLE);
}

// 读取指定ADC通道的值（轮询方式）
uint16_t Get_Adc(uint8_t CHx)
{
    // 配置规则组通道：CHx, 顺序第1个, 采样时间480周期
    ADC_RegularChannelConfig(ADC1, CHx, 1, ADC_SampleTime_480Cycles);

    // 软件触发转换
    ADC_SoftwareStartConv(ADC1);

    // 等待转换完成
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    // 返回转换结果
    return ADC_GetConversionValue(ADC1);
}