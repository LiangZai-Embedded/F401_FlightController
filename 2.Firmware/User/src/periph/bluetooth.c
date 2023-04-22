

#include "bluetooth.h"


void bluetooth_state_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd ( BLUETOOTH_STATE_GPIO_CLK,ENABLE); 

	GPIO_InitStructure.GPIO_Pin = BLUETOOTH_STATE_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(BLUETOOTH_STATE_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  bluetooth usart config
  * @param  none
  * @retval none 
  * @date   2022-03-22
  */
void bluetooth_init(void)
{
	USART_Config();
	USARTx_DMA_Rx_Config();
	USARTx_DMA_Tx_Config();
	bluetooth_state_gpio_init();
}




/**
  * @brief  bluetooth usart config
  * @param  none
  * @retval none 
  * @date   2021-02-15
  */
void USART_Config_38400(void)
{
	USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = 38400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
	USART_Init(BLUETOOTH_USART, &USART_InitStructure); 
	
  USART_DMACmd(BLUETOOTH_USART, USART_DMAReq_Rx, DISABLE); 
  USART_DMACmd(BLUETOOTH_USART, USART_DMAReq_Tx, DISABLE);                        

	
  USART_Cmd(BLUETOOTH_USART, ENABLE);
}


/**
  * @brief  bluetooth usart config
  * @param  none
  * @retval none 
  * @date   2021-02-15
  */
void USART_Config_Ui(void)
{
	USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
	USART_Init(BLUETOOTH_USART, &USART_InitStructure); 
	
  USART_DMACmd(BLUETOOTH_USART, USART_DMAReq_Rx, DISABLE); 
  USART_DMACmd(BLUETOOTH_USART, USART_DMAReq_Tx, DISABLE);                        

	
  USART_Cmd(BLUETOOTH_USART, ENABLE);
}

 

/**
  * @brief  get bluetooth config value 
  * 
  * "AT?\r\n"
  * "AT+ORGL\r\n"
  * "AT+PSWD=4321\r\n" 
  * "AT+ROLE=0\r\n"(slave mode)
  * "AT+UART=115200,0,0\r\n"
  * @param  none
  * @retval none 
  * @date   2021-02-15
  */
void bluetooth_get_config(void)
{
	char buf[20] ={0};
	uint8_t i = 0;
	
	USART_Config_38400();
	
	while(1)
  {
    printf("AT\r\n");
		while((buf[i++] = getchar())!='\r');

		cnt_timer_delay_ms(500);
		i = 0;
		memset(buf,0,sizeof(buf));
		LED_GREEN_TOGGLE();
		
		
		
	  printf("AT+ORGL\r\n");
		while((buf[i++] = getchar())!='\r');

		cnt_timer_delay_ms(500);
		i = 0;
		memset(buf,0,sizeof(buf));
		LED_GREEN_TOGGLE();
		
		
	  printf("AT+PSWD=4321\r\n");
		while((buf[i++] = getchar())!='\r');

		cnt_timer_delay_ms(500);
		i = 0;
		memset(buf,0,sizeof(buf));
		LED_GREEN_TOGGLE();
		
		
	  printf("AT+ROLE=0\r\n");
		while((buf[i++] = getchar())!='\r');

		cnt_timer_delay_ms(500);
		i = 0;
		memset(buf,0,sizeof(buf));
		LED_GREEN_TOGGLE();
		
	  
		printf("AT+UART=115200,0,0\r\n");
		while((buf[i++] = getchar())!='\r');

		cnt_timer_delay_ms(500);
		i = 0;
		memset(buf,0,sizeof(buf));
		
		
		printf("AT+NAME=FC_V3_2\r\n");
		while((buf[i++] = getchar())!='\r');

		cnt_timer_delay_ms(500);
		i = 0;
		memset(buf,0,sizeof(buf));
		
		
		LED_GREEN_ON();
		
		
      

	}

 }
