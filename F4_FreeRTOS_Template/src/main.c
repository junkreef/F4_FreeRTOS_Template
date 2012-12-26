//<one line to give the main.c name and a brief idea of what it does.>
// Copyright (C) 2012 Junpei Kishi
//
//
//This program is free software; you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License along with this program.
// If not, see <http://www.gnu.org/licenses/>.

#include "stm32f4xx.h"

/* Includes ------------------------------------------------------------------*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "kernel_support.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
portTASK_FUNCTION_PROTO(prvLED_Ctrl_1,pvParameters);
portTASK_FUNCTION_PROTO(prvLED_Ctrl_2,pvParameters);

void User_Initialize();
void LED_Init();
void USART_Setting();

/* Private function ----------------------------------------------------------*/

int main(void) {
	SystemInit();
	User_Initialize();
	FreeRTOS_Initialize();
	xTaskCreate(prvLED_Ctrl_1,"LED_Ctrl_1",128,NULL,1,NULL);
	xTaskCreate(prvLED_Ctrl_2,"LED_Ctrl_2",128,NULL,1,NULL);
	vTaskStartScheduler(); // This should never return.
	while (1)
		;
}



void User_Initialize() {
	USART_Setting();
	LED_Init();
}

void LED_Init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_Settings;
	GPIO_Settings.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Settings.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Settings.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Settings.GPIO_OType = GPIO_OType_PP;
	GPIO_Settings.GPIO_Pin = 0xf000;
	GPIO_Init(GPIOD, &GPIO_Settings);
}

portTASK_FUNCTION(prvLED_Ctrl_1,pvParameters){
	while(1){
		GPIOD->BSRRL=0x80;
		vTaskDelay(500);
		GPIOD->BSRRH=0x80;
		vTaskDelay(500);
	}
}

portTASK_FUNCTION_PROTO(prvLED_Ctrl_2,pvParameters){
	while(1){
		GPIOD->BSRRL=0x40;
		vTaskDelay(50);
		GPIOD->BSRRH=0x40;
		vTaskDelay(50);
	}
}



void USART_Setting() {
	//Supply Clock Settings
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitTypeDef GPIO_Settings;
	GPIO_Settings.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Settings.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Settings.GPIO_OType = GPIO_OType_PP;
	GPIO_Settings.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Settings.GPIO_Pin = 0x000c; //Pin2 Pin3
	GPIO_Init(GPIOA, &GPIO_Settings);

	USART_InitTypeDef USART_Settings;
	USART_Settings.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Settings.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Settings.USART_Parity = USART_Parity_No;
	USART_Settings.USART_StopBits = USART_StopBits_0_5;
	USART_Settings.USART_WordLength = USART_WordLength_8b;
	USART_Settings.USART_BaudRate = 115200;
	USART_Init(USART2, &USART_Settings);
	USART_Cmd(USART2, ENABLE);
}
