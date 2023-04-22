



#include "pwm.h"

#define  PWM_OUT_MAX 1000


/**
 * @brief  Initialize PWM  21KHz
 * @param  none
 * @retval none 
 * @date   2020-06-30
 */
void motor_pwm_init(void)
{		 					 
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	RCC_PWM_TIM_ClockCmd(RCC_PWM_TIM_CLK,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_PWM_GPIO_CLK, ENABLE); 	
	
	GPIO_PinAFConfig(PWM_GPIO,GPIO_PWM_CH1_PinSource,GPIO_PWM_AF); 
	GPIO_PinAFConfig(PWM_GPIO,GPIO_PWM_CH2_PinSource,GPIO_PWM_AF); 
	GPIO_PinAFConfig(PWM_GPIO,GPIO_PWM_CH3_PinSource,GPIO_PWM_AF); 
	GPIO_PinAFConfig(PWM_GPIO,GPIO_PWM_CH4_PinSource,GPIO_PWM_AF); 

	GPIO_InitStructure.GPIO_Pin = GPIO_PWM_PINS;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     
	GPIO_Init(PWM_GPIO,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=(2-1);  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=(1000-1);   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(PWM_TIM,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	TIM_OC1Init(PWM_TIM, &TIM_OCInitStructure); 
	TIM_OC2Init(PWM_TIM, &TIM_OCInitStructure); 
	TIM_OC3Init(PWM_TIM, &TIM_OCInitStructure); 
	TIM_OC4Init(PWM_TIM, &TIM_OCInitStructure); 

  TIM_OC1PreloadConfig(PWM_TIM, TIM_OCPreload_Enable); 
  TIM_OC2PreloadConfig(PWM_TIM, TIM_OCPreload_Enable); 
  TIM_OC3PreloadConfig(PWM_TIM, TIM_OCPreload_Enable); 
  TIM_OC4PreloadConfig(PWM_TIM, TIM_OCPreload_Enable); 

  TIM_ARRPreloadConfig(PWM_TIM,ENABLE);
	
	TIM_Cmd(PWM_TIM, ENABLE); 
}

/**
 * @brief  Limited PWM Output
 * @param  pwms
 * @retval none 
 * @date   2020-10-24
 */
void motor_pwm_out(int32_t pwm1,int32_t pwm2,int32_t pwm3,int32_t pwm4)
{
	if(pwm1>PWM_OUT_MAX)
	  pwm1 = PWM_OUT_MAX;
	
	if(pwm2>PWM_OUT_MAX)
		pwm2 = PWM_OUT_MAX;	
	
	if(pwm3>PWM_OUT_MAX)
		pwm3 = PWM_OUT_MAX;	
	
	if(pwm4>PWM_OUT_MAX)
		pwm4 = PWM_OUT_MAX;
	
	
	if(pwm1<0)
	  pwm1 = 0;
	
	if(pwm2<0)
	  pwm2 = 0;
	
	if(pwm3<0)
	  pwm3 = 0;
	
	if(pwm4<0)
	  pwm4 = 0;
	
	
	
	
  TIM_SetCompare1(PWM_TIM,pwm1);
  TIM_SetCompare2(PWM_TIM,pwm2);
	TIM_SetCompare3(PWM_TIM,pwm3);
	TIM_SetCompare4(PWM_TIM,pwm4);
}

/**
 * @brief  Stop Motors
 * @param  none
 * @retval none 
 * @date   2020-10-24
 */
void motor_stop(void)
{
	motor_pwm_out(0,0,0,0);
}


/**
 * @brief  motor controller
 * @param  throttle,pitch_pwm,roll_pwm,yaw_pwm
 * @retval none 
 * @date   2020-10-24
 */

void motor_ctrl(int32_t throttle,int32_t pitch_pwm,int32_t roll_pwm,int32_t yaw_pwm)
{
	int32_t pwm1 = 0,pwm2 = 0,pwm3 = 0,pwm4 = 0;
	
	pwm1 = throttle + roll_pwm + pitch_pwm;
	pwm2 = throttle - roll_pwm + pitch_pwm;
	pwm3 = throttle + roll_pwm - pitch_pwm;
	pwm4 = throttle - roll_pwm - pitch_pwm;

	motor_pwm_out(pwm1, pwm2, pwm3, pwm4);
	
}





