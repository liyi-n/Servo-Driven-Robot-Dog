#include "PWM_HAL.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
void Error_Handler(void);
//PWM、Servo、Movement三个文件共同为驱动舵机服务
//PA6：右前脚，PA7：右后脚，PB0：左前脚，PB1：左后脚

extern TIM_HandleTypeDef htim3;

/**
  * 函    数：PWM初始化
  * 参    数：无
  * 返 回 值：无
  */
void PWM_Init(void)
{
    /* 定时器基础配置 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 72 - 1;              // 预分频器，72MHz / 72 = 1MHz
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP; // 向上计数模式
    htim3.Init.Period = 20000 - 1;               // 计数周期，1MHz / 20000 = 50Hz
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 时钟分频
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }

    /* 配置PWM通道1 (PA6) */
    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;                         // 初始占空比为0
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;  // 输出极性为高
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }

    /* 配置PWM通道2 (PA7) */
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }

    /* 配置PWM通道3 (PB0) */
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }

    /* 配置PWM通道4 (PB1) */
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler();
    }

    /* 启动所有PWM通道 */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}

/**
  * 函    数：PWM HAL库初始化回调函数
  * 参    数：htim 定时器句柄
  * 返 回 值：无
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(htim->Instance == TIM3)
    {
        /* 使能时钟 */
        __HAL_RCC_TIM3_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /* GPIO配置 - PA6 (TIM3_CH1) */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* GPIO配置 - PB0 (TIM3_CH3), PB1 (TIM3_CH4) */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

/**
  * 函    数：PWM设置CCR - 通道1
  * 参    数：Compare 要写入的CCR的值，范围：0~20000
  * 返 回 值：无
  */
void PWM_SetCompare1(uint16_t Compare)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Compare);
}

/**
  * 函    数：PWM设置CCR - 通道2
  * 参    数：Compare 要写入的CCR的值，范围：0~20000
  * 返 回 值：无
  */
void PWM_SetCompare2(uint16_t Compare)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, Compare);
}

/**
  * 函    数：PWM设置CCR - 通道3
  * 参    数：Compare 要写入的CCR的值，范围：0~20000
  * 返 回 值：无
  */
void PWM_SetCompare3(uint16_t Compare)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, Compare);
}

/**
  * 函    数：PWM设置CCR - 通道4
  * 参    数：Compare 要写入的CCR的值，范围：0~20000
  * 返 回 值：无
  */
void PWM_SetCompare4(uint16_t Compare)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, Compare);
}

/**
  * 函    数：错误处理
  * 参    数：无
  * 返 回 值：无
  */

