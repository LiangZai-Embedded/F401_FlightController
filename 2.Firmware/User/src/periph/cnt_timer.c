

#include "cnt_timer.h" 


/**
  * @brief  count timer initialization
  * @param  none
  * @retval none
  * @date   2021-1-10
  */
void cnt_timer_init(void)
{		 					 
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  
  RCC_COUNT_TIM_ClockCmd(RCC_COUNT_TIM_CLK,ENABLE);  	
  
  TIM_TimeBaseStructure.TIM_Prescaler=(42-1);  
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
  TIM_TimeBaseStructure.TIM_Period=(5000000-1);   
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
  
  TIM_TimeBaseInit(COUNT_TIM,&TIM_TimeBaseStructure);
  
  TIM_Cmd(COUNT_TIM, DISABLE);
}

/**
  * @brief  start count timer
  * @param  none
  * @retval none
  * @date   2021-1-10
  */
void cnt_timer_start(void)
{
  COUNT_TIM->CR1 |= TIM_CR1_CEN;   //Enable the TIM Counter
  
  COUNT_TIM->CNT = 0;              //clear present value 
}


/**
  * @brief  start count timer
  * @param  none
  * @retval xus
  * @date   2021-1-10
  */
uint32_t cnt_timer_get_us(void)
{
  
  return TIM_GetCounter(COUNT_TIM);
}

/**
  * @brief  count timer delay some us
  * @param  xus: us to delay
  * @retval none
  * @date   2021-1-15
  */
void cnt_timer_delay_us(uint32_t xus)
{
   COUNT_TIM->CR1 |= TIM_CR1_CEN;   //Enable the TIM Counter
  
   COUNT_TIM->CNT = 0;              //clear present value 

   while((COUNT_TIM->CNT)!=xus);    //delay

   COUNT_TIM->CR1 &= ~TIM_CR1_CEN;  //Disable the TIM Counter
}



