#include "stm32f1xx_hal.h"                  // Device header
#include "Delay_HAL.h"
#include "Timer_HAL.h"
#include "Movement.h"
//***超声避障***//
extern uint16_t T;  //记录距离
int cf=0;//记录重复次数 解决bug用

uint16_t Time; //记录时间
void UltrasonicWave_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();	//开启GPIOA的时钟

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;

	// PA0 TRIG - 输出
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA1 ECHO - 输入（下拉）
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
}

void UltrasonicWave_Start(void)
{
	//给触发信号
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	Delay_HAL_us(45);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

	Timer_Init();  //计时 返回时间计算距离
}

uint16_t UltrasonicWave_Getvalue(void)
{
	UltrasonicWave_Start();
	Delay_HAL_ms(100);
	return ((Time * 0.0001) * 34000) / 2;
}

void Bizhang(void)
{
	T=UltrasonicWave_Getvalue();
	Delay_HAL_ms(10);
	if(T>=15)
	{
		{
		move_forward();
		//Delay_ms(10);
		cf++;
		}
	}
	else
	{
		//move_behind();
		T=UltrasonicWave_Getvalue();
	while(T<15)
	{	cf=0;
		move_right();//一次约30°
		move_right();
		Delay_HAL_ms(10);
		T=UltrasonicWave_Getvalue();
		if(T>15)
		{
			break;
		}
	}
}
}
