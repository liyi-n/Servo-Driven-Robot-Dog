/*
 * usart2_hal.h
 *
 *  Created on: Dec 4, 2025
 *      Author: ly
 */

#ifndef INC_USART2_HAL_H_
#define INC_USART2_HAL_H_

#include "stm32f1xx_hal.h"

#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收

void USART2_Init(void);
void USART2_SendByte(uint8_t Byte);
void USART2_SendString(uint8_t *String, uint16_t Length);
void USART2_SendNumber(uint32_t Number, uint8_t Length);
void USART2_Printf(char *format, ...);
uint8_t USART2_GetRxFlag(void);
uint8_t USART2_GetRxData(void);


#endif /* INC_USART2_HAL_H_ */
