/*
 * delay.h
 *
 *  Created on: 2012/10/25
 *      Author: Junpei
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"

void delay_us(uint16_t waittime);
void delay_ms(uint16_t waittime);
void delay_init();



#endif /* DELAY_H_ */
