
#define ADR_WIDTH 5

//Define the commands for operate the nRF24L01P
#define READ_nRF_REG    0x00  // Command for read register
#define WRITE_nRF_REG   0x20 	// Command for write register
#define RD_RX_PLOAD     0x61  // Command for read Rx payload
#define WR_TX_PLOAD     0xA0  // Command for write Tx payload
#define FLUSH_TX        0xE1 	// Command for flush Tx FIFO
#define FLUSH_RX        0xE2  // Command for flush Rx FIFO
#define REUSE_TX_PL     0xE3  // Command for reuse Tx payload
#define NOP             0xFF  // Reserve

//Define the register address for nRF24L01P
#define CONFIG          0x00  //  Configurate the status of transceiver, mode of CRC and the replay of transceiver status
#define EN_AA           0x01  //  Enable the atuo-ack in all channels
#define EN_RXADDR       0x02  //  Enable Rx Address
#define SETUP_AW        0x03  // Configurate the address width
#define SETUP_RETR      0x04  //  setup the retransmit
#define RF_CH           0x05  // Configurate the RF frequency
#define RF_SETUP        0x06  // Setup the rate of data, and transmit power
#define NRFRegSTATUS    0x07  //
#define OBSERVE_TX      0x08  //
#define CD              0x09  // Carrier detect
#define RX_ADDR_P0      0x0A  // Receive address of channel 0
#define RX_ADDR_P1      0x0B  // Receive address of channel 1
#define RX_ADDR_P2      0x0C  // Receive address of channel 2
#define RX_ADDR_P3      0x0D  // Receive address of channel 3
#define RX_ADDR_P4      0x0E  // Receive address of channel 4
#define RX_ADDR_P5      0x0F  // Receive address of channel 5
#define TX_ADDR         0x10  //       Transmit address
#define RX_PW_P0        0x11  //  Size of receive data in channel 0
#define RX_PW_P1        0x12  //  Size of receive data in channel 1
#define RX_PW_P2        0x13  //  Size of receive data in channel 2
#define RX_PW_P3        0x14  //  Size of receive data in channel 3
#define RX_PW_P4        0x15  // Size of receive data in channel 4
#define RX_PW_P5        0x16  //  Size of receive data in channel 5
#define FIFO_STATUS     0x17  // FIFO Status



//Define the layer1:HW operation
unsigned char NRF24l01_Send_Byte(unsigned char dat);
void NRF24l01_Init(void);
//void NRF24l01_Init_Soft(void);
void NRF24l01_NSS_L(void);
void NRF24l01_NSS_H(void);
void NRF24l01_CE_L(void);
void NRF24l01_CE_H(void);
//Define the layer2:Reg operation
unsigned char NRF24l01_WR_Reg(unsigned char reg, unsigned char value);
unsigned char NRF24l01_RD_Reg(unsigned char reg);
unsigned char NRF24l01_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len);
unsigned char NRF24l01_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len);


//Define the layer3:application operation
void NRF24l01_TX_Mode(void);
/****************************************

All the functions is in "nRF24l01P.h"

****************************************/

//Define the other function
void NRF24l01_Delay_us(unsigned long n);
