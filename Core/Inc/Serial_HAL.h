/*
 * Serial_HAL.h
 *
 *  Created on: Dec 3, 2025
 *      Author: ly
 */

#ifndef INC_SERIAL_HAL_H_
#define INC_SERIAL_HAL_H_

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include "usart.h"
extern UART_HandleTypeDef huart3;
void Serial_Init(void);
void Serial_SendByte1(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString1(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag1(void);
uint8_t Serial_GetRxData1(void);
uint8_t Serial_GetRxFlag3(void);
uint8_t Serial_GetRxData3(void);


#endif /* INC_SERIAL_HAL_H_ */
