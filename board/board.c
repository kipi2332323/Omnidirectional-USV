/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注 bilibili 账号：【立创开发板】，掌握最新动态
 * 不靠卖板赚钱，以培养中国工程师为己任
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-07     LCKFB-LP     first version
 */
#include <board.h>

static __IO uint32_t g_system_tick = 0;

/**
 * @brief 初始化 STM32 板级基础资源。
 */
void board_init(void)
{
    /* 配置中断向量表基址 */
#define NVIC_VTOR_MASK              0x3FFFFF80
#ifdef  VECT_TAB_RAM
    /* 向量表位于 RAM */
    SCB->VTOR  = (0x10000000 & NVIC_VTOR_MASK);
#else  /* VECT_TAB_FLASH  */
    /* 向量表位于 Flash */
    SCB->VTOR  = (0x08000000 & NVIC_VTOR_MASK);
#endif

    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick->LOAD = 0xFFFF; // 设置 SysTick 自动重装载值
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // 启动 SysTick

//  RCC_ClocksTypeDef rcc;
//  RCC_GetClocksFreq(&rcc); // 读取系统时钟频率

    (void)g_system_tick;
}

/**
 * @brief 使用 SysTick 实现微秒级延时。
 * @param _us 需要延时的微秒数。
 */
void delay_us(uint32_t _us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;

    // 计算目标时钟数 = 延时微秒数 * 每微秒对应的时钟数
    ticks = _us * (SystemCoreClock / 1000000);

    // 读取当前 SysTick 计数值
    told = SysTick->VAL;

    while (1)
    {
        // 持续刷新当前 SysTick 计数值
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;

            told = tnow;

            // 累计到目标时钟数后退出
            if (tcnt >= ticks)
                break;
        }
    }
}

/**
 * @brief 基于 `delay_us` 实现毫秒级延时。
 * @param _ms 需要延时的毫秒数。
 */
void delay_ms(uint32_t _ms) { delay_us(_ms * 1000); }

void delay_1ms(uint32_t ms) { delay_us(ms * 1000); }

void delay_1us(uint32_t us) { delay_us(us); }
