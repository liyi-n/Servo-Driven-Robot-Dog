/*
 * Delay_HAL.h
 *
 *  Created on: Dec 5, 2025
 *      Author: ly
 */

#ifndef INC_DELAY_HAL_H_
#define INC_DELAY_HAL_H_

#include "stm32f1xx_hal.h"

void Delay_HAL_us(uint32_t us);
void Delay_HAL_ms(uint32_t ms);
void Delay_HAL_s(uint32_t s);


#endif /* INC_DELAY_HAL_H_ */
