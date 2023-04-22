

#include "init.h"



/**
  * @brief  BSP  Initialization
  * @param  none
  * @retval none 
	* @date   2020-06-30
  */
void BSP_Init(void)
{
	cnt_timer_init();
	cnt_timer_delay_ms(1000);
	
  KEY_GPIO_Config();
	LED_GPIO_Config();
  LED_RED_ON();
  

  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
  	  
  bluetooth_init();

  I2C_Config();

  hmc5883l_init();

	icm20602_init();
  
  motor_pwm_init();

  flash_init();

  adc_init();
	
	
}


/**
  * @brief  Parameters  Initialization
  * @param  none
  * @retval none 
	* @date   2020-06-28
  */
void Parameters_Init(void)
{
//   kalman_filter_init(&kalman_pitch);
//   kalman_filter_init(&kalman_roll);
//   kalman_filter_init(&kalman_yaw);
//	
   pid_init();

   biquad_filter_init();
}


/**
  * @brief  com debug interface
  * @param  none
  * @retval none 
	* @date   2022-01-14
  */
void com_console_ui(void)
{
	char buf[80] = {0};
	uint32_t i = 0;
	char temp = 0;
	
	USART_Config_Ui();
	printf("\r\n\r\n\r\n\r\n\r\n>>>Hey! F401_FC_V3_1\r\n");
 	printf(">>>");

	while(1)
	{
		temp = getchar();
		
		if(temp=='\r')
		{
			printf("\r\n>>>");
			i = 0;
			
			if(strcmp(buf,"gyro")==0)
			{
				icm20602_print_data();
				printf("\r\n>>>");
			}
			
			if(strcmp(buf,"eeprom")==0)
			{
				AT24C02_param_print();
				printf("\r\n>>>");
			}
			
			memset(buf,0,sizeof(buf));
		}
		
		else
		{
			putchar(temp);
		  buf[i++] = temp;
		}
		
		
		
		if(READ_STATE()==SET)
		{
			LED_GREEN_ON();
		}
		
		else
		{
			LED_GREEN_OFF();
		}
		
	}
			
	
}
