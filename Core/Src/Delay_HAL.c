#include "stm32f1xx_hal.h"
#include "Delay_HAL.h"

/**
  * @brief  微秒级延时 - 基于SysTick定时器
  * @param  us 延时时长，单位：微秒
  * @retval 无
  */
void Delay_HAL_us(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;

    ticks = us * (SystemCoreClock / 1000000);
    told = SysTick->VAL;

    while (tcnt < ticks)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
        }
    }
}

/**
  * @brief  毫秒级延时 - 使用HAL库的HAL_Delay函数
  * @param  ms 延时时长，单位：毫秒
  * @retval 无
  */
void Delay_HAL_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/**
  * @brief  秒级延时
  * @param  s 延时时长，单位：秒
  * @retval 无
  */
void Delay_HAL_s(uint32_t s)
{
    while (s--)
    {
        Delay_HAL_ms(1000);
    }
}
