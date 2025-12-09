/*
 * PWM_HAL.h
 *
 *  Created on: Dec 3, 2025
 *      Author: ly
 */
#include "stm32f1xx_hal.h"
#ifndef INC_PWM_HAL_H_
#define INC_PWM_HAL_H_

/* 函数声明 */
void PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);
void PWM_SetCompare4(uint16_t Compare);

#endif /* INC_PWM_HAL_H_ */
