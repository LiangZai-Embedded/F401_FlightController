



#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"
#include "stdlib.h"
#include <stdarg.h> 

char  bluetooth_rx_buf[USART_RBUFF_SIZE] = {0};
char  bluetooth_tx_buf[USART_TBUFF_SIZE] = {0};


 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = BLUETOOTH_USART_RX_DMA_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  
	NVIC_Init(&NVIC_InitStructure);
}







/**
  * @brief  USART Configuration
  * @param  none
  * @retval none
  * @date   
  */
void USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
		
  RCC_AHB1PeriphClockCmd(BLUETOOTH_USART_RX_GPIO_CLK|BLUETOOTH_USART_TX_GPIO_CLK,ENABLE);
  BLUETOOTH_USART_RCC_ClockCmd(BLUETOOTH_USART_CLK, ENABLE);
  
	GPIO_PinAFConfig(BLUETOOTH_USART_RX_GPIO_PORT,BLUETOOTH_USART_RX_SOURCE,BLUETOOTH_USART_RX_AF);
  GPIO_PinAFConfig(BLUETOOTH_USART_TX_GPIO_PORT,BLUETOOTH_USART_TX_SOURCE,BLUETOOTH_USART_TX_AF);
  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = BLUETOOTH_USART_TX_PIN  ;  
  GPIO_Init(BLUETOOTH_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = BLUETOOTH_USART_RX_PIN;
  GPIO_Init(BLUETOOTH_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 
  
	USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = BLUETOOTH_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
	USART_Init(BLUETOOTH_USART, &USART_InitStructure);
	
	USART_DMACmd(BLUETOOTH_USART,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(BLUETOOTH_USART, ENABLE);
}

/**
  * @brief  USART RX DMA Configuration
  * @param  none
  * @retval none
  * @date   2021-02-02
  */
void USARTx_DMA_Rx_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  
	DMA_DeInit(BLUETOOTH_USART_RX_DMA_STREAM);
  while (DMA_GetCmdStatus(BLUETOOTH_USART_RX_DMA_STREAM) != DISABLE){}
  
  
	DMA_InitStructure.DMA_Channel = USART_RX_DMA_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)bluetooth_rx_buf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = USART_RBUFF_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; 
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;    
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    
  
	DMA_Init(BLUETOOTH_USART_RX_DMA_STREAM, &DMA_InitStructure);
  	
	NVIC_Configuration();

  DMA_ITConfig(BLUETOOTH_USART_RX_DMA_STREAM,DMA_IT_TC,ENABLE);
	
	DMA_ClearFlag(BLUETOOTH_USART_RX_DMA_STREAM,DMA_FLAG_TCIF5);
	
	DMA_ClearITPendingBit(BLUETOOTH_USART_RX_DMA_STREAM,DMA_IT_TCIF5);

  DMA_Cmd (BLUETOOTH_USART_RX_DMA_STREAM,ENABLE);
}


/**
  * @brief  USART TX DMA Configuration
  * @param  none
  * @retval none
  * @date   2021-02-02
  */
void USARTx_DMA_Tx_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  
	DMA_DeInit(BLUETOOTH_USART_TX_DMA_STREAM);
  while (DMA_GetCmdStatus(BLUETOOTH_USART_TX_DMA_STREAM) != DISABLE){}
  
  
	DMA_InitStructure.DMA_Channel = USART_TX_DMA_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)bluetooth_tx_buf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;;
  DMA_InitStructure.DMA_BufferSize = USART_TBUFF_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;    
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    
  
	DMA_Init(BLUETOOTH_USART_TX_DMA_STREAM, &DMA_InitStructure);
}

/**
  * @brief  Send Buffer using DMA 
  * @param  tx_buf: data buffer to send
  * @retval none
  * @date   2021-02-02
  */
void USARTx_DMA_tx(uint8_t* buf)
{
	memcpy(bluetooth_tx_buf,buf,USART_TBUFF_SIZE);
	
  USART_DMACmd(BLUETOOTH_USART,USART_DMAReq_Tx,ENABLE);                        
    
	DMA_Cmd(BLUETOOTH_USART_TX_DMA_STREAM, ENABLE);                                   
  while( DMA_GetFlagStatus(BLUETOOTH_USART_TX_DMA_STREAM, DMA_FLAG_TCIF7) == RESET);   
  
  DMA_Cmd(BLUETOOTH_USART_TX_DMA_STREAM, DISABLE);                                     
  DMA_ClearFlag(BLUETOOTH_USART_TX_DMA_STREAM, DMA_FLAG_TCIF7);  
}


void USART_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx,ch);
		
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


void USART_SendBuff(USART_TypeDef* USARTx,const uint8_t* buf,uint16_t cnt)
{
	while(cnt--)
	{
		USART_SendByte(USARTx,*buf++);
	}
}




//int fputc(int ch, FILE *f)
//{
//		USARTx_DMA_tx((const char*)&ch,1);
//		return (ch); 
//}


int fputc(int ch, FILE *f)
{
		USART_SendData(BLUETOOTH_USART, (uint8_t) ch);
		
		while (USART_GetFlagStatus(BLUETOOTH_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(BLUETOOTH_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(BLUETOOTH_USART);
}


