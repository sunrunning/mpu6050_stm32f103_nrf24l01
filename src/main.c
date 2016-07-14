/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>

#include "uart.h"
#include "spi.h"
#include "nrf24l01.h"


static volatile uint32_t gDelaycounter;

void Sys_Init(void)
{
	SysTick_Config(SystemCoreClock/1000);
	return;
}

void Delay_ms(volatile uint32_t ms)
{
	gDelaycounter = ms;
	while(gDelaycounter != 0);
}

void Delaycounter_De(void)
{
	if (gDelaycounter != 0)
		gDelaycounter--;
}

void SysTick_Handler(void)
{
	Delaycounter_De();
}

void Bsp_Init(void)
{

	Usart_Init();
	//Spi_Init();
	return;
} 

int main(int argc, char *argv[])
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* Configure PD2 to mode: slow rise-time, pushpull output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // GPIO No. 2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // slow rise time
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // push-pull output
	GPIO_Init(GPIOD, &GPIO_InitStructure); // GPIOD init
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // GPIO No. 8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // slow rise time
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // push-pull output
	GPIO_Init(GPIOA, &GPIO_InitStructure); // GPIOD init
	GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
	Sys_Init();
	Bsp_Init();
	NRF24l01_Init();
	NRF24l01_TX_Mode();
	GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
	while(1)
	{
		/* GPIO PC12 set, pin=high, LED_E off */
		//GPIOD->BSRR = GPIO_BSRR_BS6;
		GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_SET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
		/* delay --> compiler optimizer settings must be "-O0" */
		Delay_ms(500);
		
		/* GPIO PC12 reset, pin=low, LED_E on */
		//GPIOD->BSRR = GPIO_BSRR_BR6;
		GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_RESET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
		/* delay --> compiler optimizer settings must be "-O0" */
		Delay_ms(500);

		printf("testing\n\r");
	} 

}
