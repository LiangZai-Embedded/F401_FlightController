

#include "spi.h"

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  SPI_ERROR("SPI TIMEOUT! errorCode = %d",errorCode);
  
	return 0;
}

void SPI_Config(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(SPI_RCC_GPIO_CLK, ENABLE);
  
  GPIO_PinAFConfig(SPI_GPIO,SPI_SCK_PinSource,SPI_GPIO_AF);
  GPIO_PinAFConfig(SPI_GPIO,SPI_MOSI_PinSource,SPI_GPIO_AF);
  GPIO_PinAFConfig(SPI_GPIO,SPI_MISO_PinSource,SPI_GPIO_AF);
  
  GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN|SPI_MISO_PIN|SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF;       
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_OUT;       
  GPIO_Init(SPI_GPIO, &GPIO_InitStructure);

  CS_HIGH();  

  
  SPI_InitTypeDef  SPI_InitStructure;
  RCC_SPI_APBx_ClockCmd(SPI_CLK,ENABLE);
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 					
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 				
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		 				
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   				
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  		
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  
  SPI_Init(SENSORS_SPI, &SPI_InitStructure);

  SPI_Cmd(SENSORS_SPI, ENABLE);
}




uint8_t SPI_WR_Byte(uint8_t byte)
{
	uint16_t SPITimeout = SPIT_FLAG_TIMEOUT;

	while (SPI_I2S_GetFlagStatus(SENSORS_SPI, SPI_I2S_FLAG_TXE) == RESET)
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	
	
	SPI_I2S_SendData(SENSORS_SPI, byte);
 
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	while (SPI_I2S_GetFlagStatus(SENSORS_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
	
	
	return SPI_I2S_ReceiveData(SENSORS_SPI);
}







