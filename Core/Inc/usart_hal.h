/*
 * usart_hal.h
 *
 *  Created on: Dec 5, 2025
 *      Author: ly
 */

#ifndef INC_USART_HAL_H_
#define INC_USART_HAL_H_

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// USART1相关宏定义
#define USART1_REC_LEN  			200  	// 定义最大接收字节数 200
#define EN_USART1_RX 			1		// 使能（1）/禁止（0）串口1接收

// USART3相关宏定义
#define USART3_REC_LEN  			200  	// 定义最大接收字节数 200
#define EN_USART3_RX 			1		// 使能（1）/禁止（0）串口3接收

// USART1函数声明
void USART1_Init(void);
void USART1_SendByte(uint8_t Byte);
void USART1_SendArray(uint8_t *Array, uint16_t Length);
void USART1_SendString(char *String);
void USART1_SendNumber(uint32_t Number, uint8_t Length);
int USART1_fputc(int ch, FILE *f);
void USART1_Printf(char *format, ...);

uint8_t USART1_GetRxFlag(void);
uint8_t USART1_GetRxData(void);

// USART3函数声明
void USART3_Init(void);
uint8_t USART3_GetRxFlag(void);
uint8_t USART3_GetRxData(void);

#endif /* INC_USART_HAL_H_ */
