#include "tim.h"
#include "PWM_HAL.h"
//PWM、Servo、Movement三个文件共同为驱动舵机服务
//Servo用于封装舵机的角度设置与读取函数

/**
  * 函    数：舵机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Servo_Init(void)
{
	PWM_Init();									//初始化舵机的底层PWM
}

/**
  * 函    数：舵机设置角度
  * 参    数：Angle 要设置的舵机角度，范围：0~180
  * 返 回 值：无
  */
void Servo_SetAngle1(float Angle)
{
	PWM_SetCompare1(Angle / 180 * 2000 + 500);	//设置占空比

}
void Servo_SetAngle2(float Angle)
{
	PWM_SetCompare2(Angle / 180 * 2000 + 500);	//设置占空比

}
void Servo_SetAngle3(float Angle)
{
	PWM_SetCompare3(Angle / 180 * 2000 + 500);	//设置占空比

}
void Servo_SetAngle4(float Angle)
{
	PWM_SetCompare4(Angle / 180 * 2000 + 500);	//设置占空比

}

/**
  * 函    数：舵机读取角度
  * 参    数：无
  * 返 回 值：舵机角度，范围：0~180
  */
uint8_t Servo_GetAngle1(void)
{
    // __HAL_TIM_GET_COMPARE(定时器句柄, 通道) → 读取对应通道的CCR值
    return (__HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1) - 500) * 180 / 2000;
}

uint8_t Servo_GetAngle2(void)
{
    return (__HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_2) - 500) * 180 / 2000;
}

uint8_t Servo_GetAngle3(void)
{
    return (__HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_3) - 500) * 180 / 2000;
}

uint8_t Servo_GetAngle4(void)
{
    return (__HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_4) - 500) * 180 / 2000;
}
