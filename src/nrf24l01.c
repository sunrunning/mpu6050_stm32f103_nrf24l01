#include "stm32f10x_spi.h"
#include "nrf24l01.h"



void NRF24l01_Delay_us(unsigned long us)
{
	unsigned char i;
	while(us--)
	{
		i = 100;
		while(i--)
		{
			__NOP();
		}
	}
}


void NRF24l10_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE );//PORTC,PORTA时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PC4 <---> CS_pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//PC4 as CS
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA4 <---> CE_pin
	GPIO_Init(GPIOA, &GPIO_InitStructure);//PA4 as CE
	
	Spi_Init();
	
}

void NRF24l01_SPI_NSS_L(void)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_4,Bit_RESET);
}

void NRF24l01_SPI_NSS_H(void)
{
	GPIO_WriteBit(GPIOC,GPIO_Pin_4,Bit_SET);
}

void NRF24l01_CE_L(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
}

void NRF24l01_CE_H(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
}


unsigned char NRF24L01_SPI_Send_Byte(unsigned char dat)
{
	/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPIx peripheral */
	SPI_I2S_SendData(SPI1, dat);

	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}

unsigned char NRF24l01_SPI_WR_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;

	NRF24l01_SPI_NSS_L();  
	NRF24l01_Delay_us(20);	// CSN low, init SPI transaction
	status = NRF24l01_SPI_Send_Byte(reg);// select register
	NRF24l01_SPI_Send_Byte(value);             // ..and write value to it..
	NRF24l01_SPI_NSS_H();                   // CSN high again

	return(status);            // return NRF24l01 status unsigned char
}

unsigned char NRF24l01_SPI_RD_Reg(unsigned char reg)
{
	unsigned char reg_val;

	NRF24l01_SPI_NSS_L();                // CSN low, initialize SPI communication...
	NRF24l01_Delay_us(20);
	NRF24l01_SPI_Send_Byte(reg);            // Select register to read from..
	reg_val = NRF24l01_SPI_Send_Byte(0);    // ..then read register value
	NRF24l01_SPI_NSS_H();                // CSN high, terminate SPI communication

	return(reg_val);        // return register value
}


unsigned char NRF24l01_SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len)
{
	unsigned int status,i;

	NRF24l01_SPI_NSS_L();                  // Set CSN low, init SPI tranaction
	NRF24l01_Delay_us(20);
	status = NRF24l01_SPI_Send_Byte(reg);  // Select register to write to and read status unsigned char

  for(i=0;i<Len;i++)
  {
     pBuf[i] = NRF24l01_SPI_Send_Byte(0);
  }

	NRF24l01_SPI_NSS_H();

	return(status);                    // return NRF24l01 status unsigned char
}


unsigned char NRF24l01_SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len)
{
	unsigned int status,i;

	NRF24l01_SPI_NSS_L();
	NRF24l01_Delay_us(20);
	status = NRF24l01_SPI_Send_Byte(reg);
	for(i=0; i<Len; i++) //
	{
		NRF24l01_SPI_Send_Byte(*pBuf);
		pBuf ++;
	}
	NRF24l01_SPI_NSS_H();    
	return(status);   
}


