#ifndef __USART_H
#define	__USART_H

#include "stm32f4xx.h"
#include <stdio.h>





#define  USART_RX_DMA_CHANNEL                      DMA_Channel_4
#define  BLUETOOTH_USART_RX_DMA_STREAM             DMA2_Stream5
#define  BLUETOOTH_USART_RX_DMA_IRQn               DMA2_Stream5_IRQn
#define  BLUETOOTH_USART_RX_DMA_IRQHandler         DMA2_Stream5_IRQHandler
      
#define  USART_TX_DMA_CHANNEL                      DMA_Channel_4
#define  BLUETOOTH_USART_TX_DMA_STREAM             DMA2_Stream7



#define  USART_DR_ADDRESS                          (&BLUETOOTH_USART->DR)
#define  USART_RBUFF_SIZE                           32
#define  USART_TBUFF_SIZE                           32


#define BLUETOOTH_USART                             USART1

#define BLUETOOTH_USART_CLK                         RCC_APB2Periph_USART1
#define BLUETOOTH_USART_BAUDRATE                    115200  

#define BLUETOOTH_USART_RCC_ClockCmd                RCC_APB2PeriphClockCmd

#define BLUETOOTH_USART_RX_GPIO_PORT                GPIOA
#define BLUETOOTH_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BLUETOOTH_USART_RX_PIN                      GPIO_Pin_10
#define BLUETOOTH_USART_RX_AF                       GPIO_AF_USART1
#define BLUETOOTH_USART_RX_SOURCE                   GPIO_PinSource10

#define BLUETOOTH_USART_TX_GPIO_PORT                GPIOA
#define BLUETOOTH_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define BLUETOOTH_USART_TX_PIN                      GPIO_Pin_9
#define BLUETOOTH_USART_TX_AF                       GPIO_AF_USART1
#define BLUETOOTH_USART_TX_SOURCE                   GPIO_PinSource9

#define BLUETOOTH_USART_IRQHandler                  USART1_IRQHandler
#define BLUETOOTH_USART_IRQ                 	    USART1_IRQn

extern char bluetooth_rx_buf[USART_RBUFF_SIZE];
extern char  bluetooth_tx_buf[USART_TBUFF_SIZE];


void USART_Config(void);
void USARTx_DMA_Rx_Config(void);
void USARTx_DMA_Tx_Config(void);
void USART_SendBuff(USART_TypeDef* USARTx,const uint8_t* buf,uint16_t cnt);



void USART_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void USARTx_DMA_tx(uint8_t* buf);


#endif
