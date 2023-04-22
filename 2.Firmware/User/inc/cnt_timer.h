#ifndef __TIMER_H
#define __TIMER_H

#include "headfiles.h"

#define    COUNT_TIM                TIM5
#define    RCC_COUNT_TIM_CLK        RCC_APB1Periph_TIM5
#define    RCC_COUNT_TIM_ClockCmd   RCC_APB1PeriphClockCmd

void cnt_timer_init(void);	
void cnt_timer_start(void);
uint32_t cnt_timer_get_us(void);
void cnt_timer_delay_us(uint32_t xus);

#define cnt_timer_delay_ms(xms)    cnt_timer_delay_us(xms*1000)



#endif
