/*
 * syn6288.h
 *
 *  Created on: Dec 4, 2025
 *      Author: ly
 */

#ifndef INC_SYN6288_H_
#define INC_SYN6288_H_
#include "usart2_hal.h"
#include "stm32f1xx_hal.h"

void SYN_FrameInfo(uint8_t Music, uint8_t *HZdata);
void YS_SYN_Set(uint8_t *Info_data);

#endif /* INC_SYN6288_H_ */
