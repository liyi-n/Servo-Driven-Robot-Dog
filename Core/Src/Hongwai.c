#include "stm32f1xx_hal.h"                  // HAL库头文件
#include "Movement.h"


//***红外按键检测***//
extern uint8_t t1 ; //存储红外信号 前
extern uint8_t t2 ; //存储红外信号 后
extern uint8_t t3 ; //存储红外信号 右
extern uint8_t t4 ; //存储红外信号 左

void hongwai_init(void) //红外初始化 红外按键检测初始化
{
	__HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOA时钟

	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;       // 输入模式
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;         // 下拉输入
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // 低速
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);       // 初始化GPIOA
}

uint8_t hongwai_get_1(void) //获取前方红外信号
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
}

uint8_t hongwai_get_2(void)//获取后方红外信号
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
}

uint8_t hongwai_get_3(void)//获取右方红外信号
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
}

uint8_t hongwai_get_4(void)//获取左方红外信号
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
}

void Edge_detect(void)
{
	t1 = hongwai_get_1();
	t2 = hongwai_get_2();
	t3 = hongwai_get_3();
	t4 = hongwai_get_4();

	if(t1 == 1)
	{
		move_behind();
		t1 = 0;
	}

	if(t2 == 1)
	{
		move_forward();
		t2 = 0;
	}

	if(t3 == 1)
	{
		move_right_hw();
        move_forward();
		move_right_hw();
		move_forward();
		t3 = 0;
	}

	if(t4 == 1)
	{
		move_left_hw();
        move_forward();
		move_left_hw();
		move_forward();
		t4 = 0;
	}
}
