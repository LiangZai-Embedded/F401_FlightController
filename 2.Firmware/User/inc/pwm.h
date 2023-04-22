#ifndef _PWM_H
#define _PWM_H
						  
#include "headfiles.h" 	
 


#define   PWM_GPIO              GPIOA
#define   RCC_PWM_GPIO_CLK      RCC_AHB1Periph_GPIOA

#define   GPIO_PWM_PINS         GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3

#define   GPIO_PWM_CH1_PinSource     GPIO_PinSource0
#define   GPIO_PWM_CH2_PinSource     GPIO_PinSource1
#define   GPIO_PWM_CH3_PinSource     GPIO_PinSource2
#define   GPIO_PWM_CH4_PinSource     GPIO_PinSource3

#define   GPIO_PWM_AF           GPIO_AF_TIM2


#define   PWM_TIM               TIM2
#define   RCC_PWM_TIM_CLK       RCC_APB1Periph_TIM2
#define   RCC_PWM_TIM_ClockCmd  RCC_APB1PeriphClockCmd





void motor_pwm_init(void);
void motor_stop(void);
void motor_ctrl(int32_t throttle,int32_t pitch_pwm,
	              int32_t roll_pwm,int32_t yaw_pwm);
void motor_pwm_out(int32_t pwm1,int32_t pwm2,int32_t pwm3,int32_t pwm4);


#endif
