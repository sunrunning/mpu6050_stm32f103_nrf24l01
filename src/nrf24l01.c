#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "nrf24l01.h"
#include "spi.h"




//define the initial Address
unsigned char  TX_ADDRESS[ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};
unsigned char  RX_ADDRESS[ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};






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


void NRF24l01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );//PORTA时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA4 <---> CE_pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//PA4 as CE
	
	Spi_Init();
	
}

void NRF24l01_NSS_L(void)
{
	SPI1_NSS_L();
}

void NRF24l01_NSS_H(void)
{
	SPI1_NSS_H();
}

void NRF24l01_CE_L(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
}

void NRF24l01_CE_H(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
}


unsigned char NRF24l01_Send_Byte(unsigned char dat)
{
	return SPI1_Send_Byte(dat);
}

unsigned char NRF24l01_WR_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;

	NRF24l01_NSS_L();  
	NRF24l01_Delay_us(20);	// CSN low, init SPI transaction
	status = NRF24l01_Send_Byte(reg);// select register
	NRF24l01_Send_Byte(value);             // ..and write value to it..
	NRF24l01_NSS_H();                   // CSN high again

	return(status);            // return NRF24l01 status unsigned char
}

unsigned char NRF24l01_RD_Reg(unsigned char reg)
{
	unsigned char reg_val;

	NRF24l01_NSS_L();                // CSN low, initialize SPI communication...
	NRF24l01_Delay_us(20);
	NRF24l01_Send_Byte(reg);            // Select register to read from..
	reg_val = NRF24l01_Send_Byte(0);    // ..then read register value
	NRF24l01_NSS_H();                // CSN high, terminate SPI communication

	return(reg_val);        // return register value
}


unsigned char NRF24l01_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len)
{
	unsigned int status,i;

	NRF24l01_NSS_L();                  // Set CSN low, init SPI tranaction
	NRF24l01_Delay_us(20);
	status = NRF24l01_Send_Byte(reg);  // Select register to write to and read status unsigned char

  for(i=0;i<Len;i++)
  {
     pBuf[i] = NRF24l01_Send_Byte(0);
  }

	NRF24l01_NSS_H();

	return(status);                    // return NRF24l01 status unsigned char
}


unsigned char NRF24l01_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len)
{
	unsigned int status,i;

	NRF24l01_NSS_L();
	NRF24l01_Delay_us(20);
	status = NRF24l01_Send_Byte(reg);
	for(i=0; i<Len; i++) //
	{ 
		NRF24l01_Send_Byte(*pBuf);
		pBuf ++;
	}
	NRF24l01_NSS_H();    
	return(status);   
}


void NRF24l01_TX_Mode(void)
{
	NRF24l01_CE_L();

	NRF24l01_WR_Reg(WRITE_nRF_REG + SETUP_AW, 0x03); // setup add width 5 bytes
	NRF24l01_Delay_us(20);
	NRF24l01_WR_Reg(WRITE_nRF_REG + CONFIG, 0x38); // enable power up and ptx
	NRF24l01_Delay_us(20);
	NRF24l01_WR_Reg(WRITE_nRF_REG + EN_RXADDR, 0x01); //Enable data P0
	NRF24l01_Delay_us(20);
	NRF24l01_WR_Reg(WRITE_nRF_REG + SETUP_RETR, 0x00);//Auto Retransmit Delay: 500 us, Auto Retransmit Count: Up to 2 Re-Transmit
	NRF24l01_Delay_us(20);
	NRF24l01_WR_Reg(WRITE_nRF_REG + RF_CH,0x02);// setup frequency
	NRF24l01_Delay_us(20);
	NRF24l01_WR_Reg(WRITE_nRF_REG + RF_SETUP ,0x07); //setup power 0dbm, rate 1Mbps
	NRF24l01_Delay_us(20);
	NRF24l01_Write_Buf(WRITE_nRF_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH); // write address into tx_add
	NRF24l01_Delay_us(20);
	NRF24l01_Write_Buf(WRITE_nRF_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH); // write address into rx_add_p0
	NRF24l01_Delay_us(20);
	NRF24l01_WR_Reg(WRITE_nRF_REG + EN_AA, 0x00);     // Disable Auto.Ack:Pipe0
	NRF24l01_Delay_us(200);
	
	NRF24l01_CE_H();
	
}
