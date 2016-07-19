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
unsigned char Tx_Buf[32]=
{
	0x01,0x02,0x03,0x4,0x05,0x06,0x07,0x08,
	0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,
	0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32,
};

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
	unsigned char status=0xa5;
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
	NRF24l01_HW_Init();
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
		NRF24l01_TX_Packet(Tx_Buf);
		Delay_ms(10);
		status=NRF24l01_RD_Reg(NRFRegSTATUS);
		printf(" status is: %d\n\r",status);
		status=NRF24l01_RD_Reg(0x17);
		printf(" fifo is: %d\n\r",status);
		status=NRF24l01_RD_Reg(RF_SETUP);
		printf(" rf setup is: %d\n\r",status);
		status=NRF24l01_RD_Reg(RF_CH);
		printf(" rf chanel is: %d\n\r",status);
		status=NRF24l01_RD_Reg(CONFIG);
		printf(" config is: %d\n\r",status);
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		NRF24l01_WR_Reg(WRITE_nRF_REG + CONFIG, 0x3B); // enable power up and prx
		
		Delay_ms(1000);
		
		
		/* GPIO PC12 reset, pin=low, LED_E on */
		//GPIOD->BSRR = GPIO_BSRR_BR6;
		GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_RESET);
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
		/* delay --> compiler optimizer settings must be "-O0" */
		Delay_ms(500);
		//ret = NRF24l01_RD_Reg(0x00);
		//printf("0x00 is 0x%X\n\r",ret);
	} 

}
