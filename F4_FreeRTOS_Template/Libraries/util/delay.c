/*
 * delay.c
 *
 *  Created on: 2012/10/25
 *      Author: Junpei
 */

#include "stm32f4xx.h"
#include "delay.h"

void delay_core();

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructureMicro;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructureMili;

void delay_init(){

	//Enable clock( (AHB_Clock/4) --> APB1_Clock)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	TIM_TimeBaseStructureMicro.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructureMicro.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructureMicro.TIM_Prescaler=42-1;
	TIM_TimeBaseStructureMicro.TIM_RepetitionCounter=0;
	TIM_TimeBaseStructureMili.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructureMili.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructureMili.TIM_Prescaler=42000-1;
	TIM_TimeBaseStructureMili.TIM_RepetitionCounter=0;
}

void delay_us(uint16_t waittime){
	TIM_TimeBaseStructureMicro.TIM_Period=waittime*2;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructureMicro);
	delay_core();
}

void delay_ms(uint16_t waittime){
	TIM_TimeBaseStructureMili.TIM_Period=waittime*2;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructureMili);
	delay_core();
}

void delay_core(){
	TIM_SetCounter(TIM2,1);
	TIM_Cmd(TIM2,ENABLE);

	while(TIM_GetCounter(TIM2));

	TIM_Cmd(TIM2,DISABLE);
}
