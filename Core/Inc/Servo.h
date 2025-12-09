/*
 * Servo.h
 *
 *  Created on: Dec 3, 2025
 *      Author: ly
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_
#include "stm32f1xx_hal.h"

void Servo_Init(void);
void Servo_SetAngle1(float Angle);
void Servo_SetAngle2(float Angle);
void Servo_SetAngle3(float Angle);
void Servo_SetAngle4(float Angle);
uint8_t Servo_GetAngle1(void);
uint8_t Servo_GetAngle2(void);
uint8_t Servo_GetAngle3(void);
uint8_t Servo_GetAngle4(void);

#endif /* INC_SERVO_H_ */
