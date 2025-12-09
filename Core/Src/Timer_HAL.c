#include "stm32f1xx_hal.h"                  // Device header
//为超声避障服务
extern uint16_t Time;

// 定义TIM2句柄为全局变量
TIM_HandleTypeDef htim2;

void Timer_Init(void)
{
	Time = 0;

	// 使能TIM2时钟
	__HAL_RCC_TIM2_CLK_ENABLE();

	// 配置TIM2
	htim2.Instance = TIM2;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;		//计数模式，此处为向上计数
	htim2.Init.Period = 7200-1;		//ARR 1 = 0.0001S
	htim2.Init.Prescaler = 0;		//PSC
	htim2.Init.RepetitionCounter = 0;		//高级计时器特有，重复计数

	// 初始化TIM2
	HAL_TIM_Base_Init(&htim2);

	// 配置TIM2中断优先级
	HAL_NVIC_SetPriority(TIM2_IRQn, 2, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	// 启动TIM2
	HAL_TIM_Base_Start_IT(&htim2);
}

void TIM2_IRQHandler(void)
{
	// 检查TIM2更新中断
	if (__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET)
	{
		// 检查PA1引脚状态
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_SET)
		{
			Time ++;
		}

		// 清除中断标志位
		__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
	}
}

